/*
**  secure/simul/parse_string.c
**
**  Parse a string and replace standard tokens.
**  Iria@Dune.
**
**  Replaced original implementation using cached lambda generation to reduce overhead.
**  Rationlised a few utterly bizarre semantics.
**  Added a few new tokens.
**  Bubbs@Dune.  31/Dec/2013.
**
**  Updated code, tweaked codes.
**  Bubbs@Dune.  2018-07-15.
**
**  Added guild tokens: %Gn, %GN, %GX, %GL
**    and party tokens: %PtN, %PtX, %PtT, %PtB
**  Math@Dune. August 16, 2020
**
** Copypasted over to Dune_3.5 - Math
*/

#include <daemons.h> // Necessary for PARTY_DAEMON and PARTY_OBJECT defines.

// parse_string_codes mapping value index and definitions.
#define PSCODES_CHECK_CODE              0    // int|mixed*    - code to validate token usage for user;
#define PSCODES_EVAL_CODE               1    // string|mixed* - code to evaluate token.
#define PARSE_STRING_CODES_WIDTH        (PSCODES_EVAL_CODE +1)


#if __DIR__ == "secure/simul/"
        status  playerp                 (object ob);
        status  wizardp                 (object ob);
        status  adminp                  (object ob);
        status  invisp                  (object ob);
        string  neat_file               (mixed ob);
        string  pronoun                 (object ob);
        string  possessive              (object ob);
        string  objective               (object ob);
        string  possessive_name         (object ob);
        string  gender                  (object ob);
varargs int     user_time               (mixed arg, int base_time);
#endif


nosave  mapping parse_string_codes    = 0;
nosave  mapping parse_string_cache    = 0;


varargs string parse_string(string str, object ob, object alt) {
  !stringp(str)         && raise_error("parse_string(): Bad type to argument 1.\n");
  !(!ob || objectp(ob)) && raise_error("parse_string(): Bad type to argument 2.\n");

  if (!parse_string_codes) {
    mixed   ob_is_admin     = ({ #'||=, 'is_admin,    ({ #'adminp,  'ob,  }), });
    mixed   ob_is_wizard    = ({ #'||=, 'is_wizard,   ({ #'wizardp, 'ob,  }), });
    mixed   ob_is_player    = ({ #'||=, 'is_player,   ({ #'playerp, 'ob,  }), });
    mixed   ob_is_living    = ({ #'||=, 'is_living,   ({ #'living,  'ob,  }), });
    mixed   alt_is_object   = ({ #'||=, 'alt_is_obj,  ({ #'objectp, 'alt, }) ,});
    mixed   alt_is_living   = ({ #'||=, 'alt_is_liv,  ({ #'&&, alt_is_object, ({ #'living, 'alt, }), }), });

    mixed   ob_name         = ({ #'||=, 'name,        ({ #'call_other, 'ob, "query_name",    }), });
    mixed   ob_hp           = ({ #'||=, 'hp,          ({ #'call_other, 'ob, "query_hp",      }), });
    mixed   ob_cp           = ({ #'||=, 'cp,          ({ #'call_other, 'ob, "query_cp",      }), });
    mixed   ob_max_hp       = ({ #'||=, 'max_hp,      ({ #'call_other, 'ob, "query_max_hp",  }), });
    mixed   ob_max_cp       = ({ #'||=, 'max_cp,      ({ #'call_other, 'ob, "query_max_cp",  }), });
    mixed   ob_hp_perc      = ({ #'||=, 'hp_perc,     ({ #'/, ({ #'*, 100, ob_hp, }), ob_max_hp, }), });
    mixed   ob_cp_perc      = ({ #'||=, 'cp_perc,     ({ #'/, ({ #'*, 100, ob_cp, }), ob_max_cp, }), });
    mixed   ob_xp           = ({ #'||=, 'xp,          ({ #'call_other, 'ob, "query_exp",     }), });
    mixed   op_xp_cap       = ({ #'||=, 'xp_cap,      ({ #'call_other, 'ob, "query_exp_cap", }), });
    mixed   ob_xp_perc      = ({ #'||=, 'xp_perc,     ({ #'to_int, ({ #'/, ({ #'*, 100.0, ob_xp, }), op_xp_cap, }), }), });
    mixed   ob_load         = ({ #'||=, 'load,        ({ #'/, ({ #'*, 100, ({ #'call_other, 'ob, "query_carry", }), }), ({ #'+, 5, ({ #'*, 2, ({ #'call_other, 'ob, "query_str", }), }), }), }), });
    mixed   ob_ctime        = ({ #'||=, 'ctime,       ({ #'ctime, ({ #'user_time, 'ob, }), }), });
    mixed   ob_guild_soul   = ({ #'||=, 'guild_soul,  ({ #'call_other, 'ob, "query_guild_soul", }), });
    mixed   ob_party_obj    = ({ #'||=, 'party_obj,   ({ #'present_clone, PARTY_OBJECT, 'ob, }), });
    mixed   ob_party_name   = ({ #'||=, 'party_name,  ({ #'&&, ob_party_obj, ({ #'call_other, 'party_obj, "query_partyname", }), }), });
    mixed   ob_path         = ({ #'||=, 'path,        ({ #'||, ({ #'call_other, 'ob, "query_path", }), "", }), });
    mixed   ob_env          = ({ #'||=, 'env,         ({ #'environment, 'ob, }), });
    mixed   ob_env_obname   = ({ #'||=, 'env_obname,  ({ #'&&, ob_env, ({ #'object_name, 'env, }), }), });
    mixed   ob_exits        = ({ #'||=, 'exits,       ({ #'||, ({ #'&&, ob_env, ({ #'call_other, 'env, "query_is_room", }), ({ #'call_other, 'env, "list_dests", 0, }), }), "[none]", }), });

    mixed   alt_name        = ({ #'||=, 'alt_name,    ({ #'call_other, 'alt, "query_name", }), });

    parse_string_codes      = ([:PARSE_STRING_CODES_WIDTH ]);

    m_add(parse_string_codes, "\\\\", 1,              "\\");
    m_add(parse_string_codes, "\\%",  1,              "%");
    m_add(parse_string_codes, "\\$",  1,              "$");
    m_add(parse_string_codes, "\\0",  1,              "");
    m_add(parse_string_codes, "\\a",  1,              "\a");
    m_add(parse_string_codes, "\\e",  1,              "\e");
    m_add(parse_string_codes, "\\f",  1,              "\f");
    m_add(parse_string_codes, "\\n",  1,              "\n");
    m_add(parse_string_codes, "\\r",  1,              "\r");
    m_add(parse_string_codes, "\\t",  1,              "\t");

    m_add(parse_string_codes, "%CR",  1,              "\n");
    m_add(parse_string_codes, "%MUD", 1,              MUD_NAME);
    m_add(parse_string_codes, "%s",   1,              " ");

    m_add(parse_string_codes, "%N",   1,              ({ #'||, ob_name, "something", }));
    m_add(parse_string_codes, "%O",   1,              ({ #'?, ob_is_living, ({ #'?, ({ #'invisp, 'ob, }), ob_name, ({ #'call_other, 'ob, "short", 1, }), }), ({ #'||, ({ #'call_other, 'ob, "short", 1, }), "something", }), }));

  //m_add(parse_string_codes, "%PN",  ob_is_living,   ({ #'+, ob_name, ({ #'?, ({ #'strlen, ({ #'&, ({ #'[<..], 'name, 1, 1, }), "Ss", }), }), "'", "'s", }), }));
    m_add(parse_string_codes, "%PN",  ob_is_living,   ({ #'possessive_name, 'ob, }));
    m_add(parse_string_codes, "%g",   ob_is_living,   ({ #'gender,          'ob, }));
    m_add(parse_string_codes, "%P",   ob_is_living,   ({ #'possessive,      'ob, }));
    m_add(parse_string_codes, "%p",   ob_is_living,   ({ #'pronoun,         'ob, }));
    m_add(parse_string_codes, "%o",   ob_is_living,   ({ #'objective,       'ob, }));

    m_add(parse_string_codes, "%h",   ob_is_living,   ({ #'to_string,  ob_hp,     }));
    m_add(parse_string_codes, "%c",   ob_is_living,   ({ #'to_string,  ob_cp,     }));
    m_add(parse_string_codes, "%H",   ob_is_living,   ({ #'to_string,  ob_max_hp, }));
    m_add(parse_string_codes, "%C",   ob_is_living,   ({ #'to_string,  ob_max_cp, }));
    m_add(parse_string_codes, "%HP",  ob_is_living,   ({ #'?, ({ #'<=, ob_max_hp, 0, }), "---", ({ #'to_string, ob_hp_perc, }), }));
    m_add(parse_string_codes, "%CP",  ob_is_living,   ({ #'?, ({ #'<=, ob_max_cp, 0, }), "---", ({ #'to_string, ob_cp_perc, }), }));

    m_add(parse_string_codes, "%G",    ob_is_living,   ({ #'to_string,  ({ #'call_other, 'ob, "query_guild",      }), }));
    m_add(parse_string_codes, "%Gn",   ob_is_living,   ({ #'to_string,  ({ #'call_other, 'ob, "query_guild",      }), }));
    m_add(parse_string_codes, "%GN",   ob_is_living,   ({ #'to_string,  ({ #'call_other, 'ob, "query_guild_name", }), }));
    m_add(parse_string_codes, "%L" ,   ob_is_living,   ({ #'to_string,  ({ #'call_other, 'ob, "query_level",      }), }));
    m_add(parse_string_codes, "%m" ,   ob_is_living,   ({ #'to_string,  ({ #'call_other, 'ob, "query_bank_money", }), }));
    m_add(parse_string_codes, "%W" ,   ob_is_living,   ({ #'to_string,  ({ #'call_other, 'ob, "query_wimpy",      }), }));
    m_add(parse_string_codes, "%M" ,   ob_is_living,   ({ #'to_string,  ({ #'call_other, 'ob, "query_money",      }), }));
    m_add(parse_string_codes, "%E" ,   ob_is_living,   ({ #'to_string,  ob_xp,     }));
    m_add(parse_string_codes, "%EC",   ob_is_living,   ({ #'to_string,  op_xp_cap, }));
    m_add(parse_string_codes, "%EP",   ob_is_living,   ({ #'?, ({ #'<=, op_xp_cap, 0, }), "---", ({ #'to_string, ob_xp_perc, }), }));
    m_add(parse_string_codes, "%LP",   ob_is_living,   ({ #'to_string,  ob_load,   }));

    m_add(parse_string_codes, "%GL",  ob_is_player,   ({ #'?, ({ #'objectp, ob_guild_soul, }), ({ #'to_string, ({ #'||, ({ #'call_other, 'guild_soul, "query_guild_level",         }), "", }), }), "", }));
    m_add(parse_string_codes, "%GX",  ob_is_player,   ({ #'?, ({ #'objectp, ob_guild_soul, }), ({ #'to_string, ({ #'||, ({ #'call_other, 'guild_soul, "query_gexp",                }), "", }), }), "", }));
    m_add(parse_string_codes, "%G0",  ob_is_player,   ({ #'?, ({ #'objectp, ob_guild_soul, }), ({ #'to_string, ({ #'||, ({ #'call_other, 'guild_soul, "parse_string_token", "%G0", }), "", }), }), "", }));
    m_add(parse_string_codes, "%G1",  ob_is_player,   ({ #'?, ({ #'objectp, ob_guild_soul, }), ({ #'to_string, ({ #'||, ({ #'call_other, 'guild_soul, "parse_string_token", "%G1", }), "", }), }), "", }));
    m_add(parse_string_codes, "%G2",  ob_is_player,   ({ #'?, ({ #'objectp, ob_guild_soul, }), ({ #'to_string, ({ #'||, ({ #'call_other, 'guild_soul, "parse_string_token", "%G2", }), "", }), }), "", }));
    m_add(parse_string_codes, "%G3",  ob_is_player,   ({ #'?, ({ #'objectp, ob_guild_soul, }), ({ #'to_string, ({ #'||, ({ #'call_other, 'guild_soul, "parse_string_token", "%G3", }), "", }), }), "", }));
    m_add(parse_string_codes, "%G4",  ob_is_player,   ({ #'?, ({ #'objectp, ob_guild_soul, }), ({ #'to_string, ({ #'||, ({ #'call_other, 'guild_soul, "parse_string_token", "%G4", }), "", }), }), "", }));
    m_add(parse_string_codes, "%G5",  ob_is_player,   ({ #'?, ({ #'objectp, ob_guild_soul, }), ({ #'to_string, ({ #'||, ({ #'call_other, 'guild_soul, "parse_string_token", "%G5", }), "", }), }), "", }));
    m_add(parse_string_codes, "%G6",  ob_is_player,   ({ #'?, ({ #'objectp, ob_guild_soul, }), ({ #'to_string, ({ #'||, ({ #'call_other, 'guild_soul, "parse_string_token", "%G6", }), "", }), }), "", }));
    m_add(parse_string_codes, "%G7",  ob_is_player,   ({ #'?, ({ #'objectp, ob_guild_soul, }), ({ #'to_string, ({ #'||, ({ #'call_other, 'guild_soul, "parse_string_token", "%G7", }), "", }), }), "", }));
    m_add(parse_string_codes, "%G8",  ob_is_player,   ({ #'?, ({ #'objectp, ob_guild_soul, }), ({ #'to_string, ({ #'||, ({ #'call_other, 'guild_soul, "parse_string_token", "%G8", }), "", }), }), "", }));
    m_add(parse_string_codes, "%G9",  ob_is_player,   ({ #'?, ({ #'objectp, ob_guild_soul, }), ({ #'to_string, ({ #'||, ({ #'call_other, 'guild_soul, "parse_string_token", "%G9", }), "", }), }), "", }));

    m_add(parse_string_codes, "%PtX", ob_is_player,   ({ #'?, ({ #'strlen, ob_party_name,  }), ({ #'to_string, ({ #'call_other, PARTY_DAEMON,  "query_experience", 'party_name, }), }), "", }));
    m_add(parse_string_codes, "%PtT", ob_is_player,   ({ #'?, ({ #'strlen, ob_party_name,  }), ({ #'to_string, ({ #'||, ({ #'call_other, PARTY_DAEMON,  "query_party_type", 'party_name, }), "", }), }), "", }));
    m_add(parse_string_codes, "%PtB", ob_is_player,   ({ #'?, ({ #'strlen, ob_party_name,  }), ({ #'?,         ({ #'call_other, PARTY_DAEMON,  "good_for_gexp_bonus", 'party_name, }), "yes", "no",  }), "", }));
    m_add(parse_string_codes, "%PtN", ob_is_player,   ({ #'?, ({ #'strlen, ob_party_name,  }), 'party_name, "", }));

    m_add(parse_string_codes, "%R",   ob_is_living,   ({ #'||, ({ #'&&, ob_env, ({ #'call_other, 'env, "short", 1, }), }), "None", }));
    m_add(parse_string_codes, "%X",   ob_is_living,   ob_exits);
    m_add(parse_string_codes, "%x",   ob_is_living,   ({ #'[..<], ({ #'regreplace, ob_exits, "\e(\\[|\\])[0-9;]*m", "", 1, }), 1, 2, }));
    m_add(parse_string_codes, "%RX",  ob_is_living,   parse_string_codes["%X", PSCODES_EVAL_CODE]);
    m_add(parse_string_codes, "%Rx",  ob_is_living,   parse_string_codes["%x", PSCODES_EVAL_CODE]);

    m_add(parse_string_codes, "%T",   ob_is_player,   ({ #'[..], ob_ctime, 11, 15, }));
    m_add(parse_string_codes, "%HMS", ob_is_player,   ({ #'[..], ob_ctime, 11, 18, }));
    m_add(parse_string_codes, "%D",   ob_is_player,   ({ #'[..], ob_ctime,  4,  9, }));
    m_add(parse_string_codes, "%Y",   ob_is_player,   ({ #'[..], ob_ctime, 20, 23, }));
    m_add(parse_string_codes, "%DMY", ob_is_player,   ({ #'regreplace, ob_ctime, "^([A-Za-z]+) +([A-Za-z]+) +([0-9]+) +([0-9:]+) +([0-9]+)$", "\\3/\\2/\\5", 0, }));
    m_add(parse_string_codes, "%MDY", ob_is_player,   ({ #'regreplace, ob_ctime, "^([A-Za-z]+) +([A-Za-z]+) +([0-9]+) +([0-9:]+) +([0-9]+)$", "\\2/\\3/\\5", 0, }));
  //m_add(parse_string_codes, "%YMD", ob_is_player,   ({ #'regreplace, ob_ctime, "^([A-Za-z]+) +([A-Za-z]+) +([0-9]+) +([0-9:]+) +([0-9]+)$", "\\5-\\2-\\3", 0, }));
    m_add(parse_string_codes, "%YMD", ob_is_player,   ({ #'sprintf, "%s-%02d-%s", ({ #'[..], ob_ctime, 20, 23, }), ({ #'/, ({ #'strstr, "   JanFebMarAprMayJunJulAugSepOctNovDec", ({ #'[..], 'ctime, 4, 6, }), }), 3, }), ({ #'[..], 'ctime, 8, 9, }), }));

    m_add(parse_string_codes, "%CN",  ob_is_player,   ({ #'to_string,  ({ #'call_other, 'ob, "query_cmd_number",      }), }));
    m_add(parse_string_codes, "%n",   ob_is_player,   ({ #'capitalize, ({ #'call_other, 'ob, "query_real_name",       }), }));
  //m_add(parse_string_codes, "%XP",  ob_is_player,   ({ #'to_string,  ({ #'call_other, 'ob, "query_explorer_points", }), }));
    m_add(parse_string_codes, "%XP",  ob_is_player,   ({ #'to_string,  ({ #'call_other, 'ob, "query_explorer_rating", }), }));
    m_add(parse_string_codes, "%XR",  ob_is_player,   ({ #'to_string,  ({ #'call_other, 'ob, "query_explorer_rating", }), }));
  //m_add(parse_string_codes, "%EM",  ob_is_player,   ({ #'to_string,  ({ #'call_other, EXPD,   "calc_exp_mod",       'ob, }), }));
  //m_add(parse_string_codes, "%Q",   ob_is_player,   ({ #'to_string,  ({ #'call_other, QUESTD, "query_quest_points", 'ob, }), }));
  //m_add(parse_string_codes, "%RK",  ob_is_player,   ({ #'to_string,  ({ #'call_other, RANKD,  "query_rank",         'ob, }), }));

    m_add(parse_string_codes, "$HP",  ob_is_player,   ({ #'?, ({ #'<=, ob_max_hp, 0, }), "", ({ #'<=, ob_hp_perc, 10, }), "$UNDERL$BLINK$BOLD$RED", ({ #'<, 'hp_perc, 30, }), "$BOLD$RED",     ({ #'<, 'hp_perc, 60, }), "$BOLD$YELLOW", ({ #'<, 'hp_perc, 80, }), "$GREEN",        ({ #'<, 'hp_perc, 100, }), "$BOLD$GREEN", "$UNDERL$BOLD$GREEN", }));
    m_add(parse_string_codes, "$CP",  ob_is_player,   ({ #'?, ({ #'<=, ob_max_cp, 0, }), "", ({ #'<=, ob_cp_perc, 10, }), "$UNDERL$BOLD$MAGENTA",   ({ #'<, 'cp_perc, 30, }), "$BOLD$MAGENTA", ({ #'<, 'cp_perc, 60, }), "$BOLD$BLUE",   ({ #'<, 'cp_perc, 80, }), "$CYAN",         ({ #'<, 'cp_perc, 100, }), "$BOLD$CYAN",  "$UNDERL$BOLD$CYAN",  }));
    m_add(parse_string_codes, "$EP",  ob_is_player,   ({ #'?, ({ #'<=, op_xp_cap, 0, }), "", ({ #'<=, ob_xp_perc, 10, }), "$WHITE",                 ({ #'<, 'xp_perc, 30, }), "$BOLD$WHITE",   ({ #'<, 'xp_perc, 60, }), "$MAGENTA",     ({ #'<, 'xp_perc, 80, }), "$BOLD$MAGENTA", ({ #'<, 'xp_perc, 100, }), "$RED",        "$UNDERL$BOLD$RED",   }));

    m_add(parse_string_codes, "%I",   ob_is_wizard,   ({ #'?, ({ #'invisp, 'ob, }), "*", "", }));
    m_add(parse_string_codes, "%wd",  ob_is_wizard,   ({ #'neat_file, ob_path, }));
    m_add(parse_string_codes, "%WD",  ob_is_wizard,   ({ #'+, ({ #'?, ({ #'strlen, ({ #'&, ({ #'[..], ob_path, 0, 0, }), "/", }), }), "", "/", }), 'path, }));
#if __EFUN_DEFINED__(creator)
    m_add(parse_string_codes, "%RC",  ob_is_wizard,   ({ #'||, ({ #'&&, ob_env, ({ #'creator, 'env, }), }), MUD_NAME, }));
#elif __EFUN_DEFINED__(getuid)
    m_add(parse_string_codes, "%RC",  ob_is_wizard,   ({ #'||, ({ #'&&, ob_env, ({ #'getuid, 'env, }), }), MUD_NAME, }));
#endif
    m_add(parse_string_codes, "%OB",  ob_is_wizard,   ({ #'object_name, 'ob, }));
    m_add(parse_string_codes, "%AOB", ob_is_wizard,   ({ #'?, alt_is_object, ({ #'object_name, 'alt, }), "", }));
    m_add(parse_string_codes, "%rob", ob_is_wizard,   ({ #'?, ob_env, ({ #'neat_file,   'env, }), "<nowhere>", }));
    m_add(parse_string_codes, "%ROB", ob_is_wizard,   ({ #'?, ob_env_obname, ({ #'+, ({ #'?, ({ #'strlen, ({ #'&, ({ #'[..], 'env_obname, 0, 0, }), "/@", }), }), "", "/", }), 'env_obname, }), "<nowhere>", }));

    m_add(parse_string_codes, "%SI",  ob_is_admin,    ({ #'?, ({ #'||=, 'snooper, ({ #'query_snoop, 'ob, }), }), "@", "", }));
    m_add(parse_string_codes, "%SN",  ob_is_admin,    ({ #'?, ({ #'||=, 'snooper, ({ #'query_snoop, 'ob, }), }), ({ #'regreplace, ({ #'regreplace, ({ #'to_string, ({ #'call_other, 'snooper, "query_real_name", }), }), "^(0|)$", ({ #'object_name, 'snooper, }), 1, }), "^.*$", "[&]", 0, }), "",}));
    m_add(parse_string_codes, "%ASI",  ob_is_admin,   ({ #'?, ({ #'||=, 'alt_snooper, ({ #'&&, alt_is_object, ({ #'query_snoop, 'alt, }), }), }), "@", "", }));
    m_add(parse_string_codes, "%ASN",  ob_is_admin,   ({ #'?, ({ #'||=, 'alt_snooper, ({ #'&&, alt_is_object, ({ #'query_snoop, 'alt, }), }), }), ({ #'regreplace, ({ #'regreplace, ({ #'to_string, ({ #'call_other, 'alt_snooper, "query_real_name", }), }), "^(0|)$", ({ #'object_name, 'alt_snooper, }), 1, }), "^.*$", "[&]", 0, }), "",}));
    m_add(parse_string_codes, "%_PC", ob_is_admin,    ({ #'to_string, ({ #'sizeof, ({ #'parse_string_cache, }), }), }));
    m_add(parse_string_codes, "%_PR", ob_is_admin,    ({ #'||, ({ #'[, ({ #'parse_string_cache, }), 0, }), "", }));

#if __EFUN_DEFINED__(creator)
  //m_add(parse_string_codes, "%AC",  ({ #'&&, ob_is_wizard, alt_is_object, }), ({ #'||, ({ #'creator, 'alt, }), MUD_NAME, }));
    m_add(parse_string_codes, "%AC",  ob_is_wizard,   ({ #'?, alt_is_object, ({ #'||, ({ #'creator, 'alt, }), MUD_NAME, }), "%AC", }));
#elif __EFUN_DEFINED__(getuid)
    m_add(parse_string_codes, "%AC",  ob_is_wizard,   ({ #'?, alt_is_object, ({ #'||, ({ #'getuid, 'alt, }), MUD_NAME, }), "%AC", }));
#endif

    m_add(parse_string_codes, "%ALN", alt_is_object,  ({ #'lower_case, ({ #'||, alt_name, "something", }), }));
    m_add(parse_string_codes, "%AN",  alt_is_object,  ({ #'||, alt_name, "something", }));
    m_add(parse_string_codes, "%AO",  alt_is_object,  ({ #'?, alt_is_living, ({ #'?, ({ #'invisp, 'alt, }), alt_name, ({ #'call_other, 'alt, "short", 1, }), }), ({ #'||, ({ #'call_other, 'alt, "short", 1, }), "something", }), }));

    m_add(parse_string_codes, "%APN", alt_is_object,  ({ #'?, ({ #'stringp, alt_name, }), ({ #'+, 'alt_name, ({ #'?, ({ #'strlen, ({ #'&, ({ #'[<..], 'alt_name, 1, 1, }), "Ss", }), }), "'", "'s", }), }), "something's", }));
    m_add(parse_string_codes, "%Ag",  alt_is_object,  ({ #'||, ({ #'gender,     'alt, }), "neutral", }));
    m_add(parse_string_codes, "%AP",  alt_is_object,  ({ #'||, ({ #'possessive, 'alt, }), "its",     }));
    m_add(parse_string_codes, "%Ap",  alt_is_object,  ({ #'||, ({ #'pronoun,    'alt, }), "it",      }));
    m_add(parse_string_codes, "%Ao",  alt_is_object,  ({ #'||, ({ #'objective,  'alt, }), "it",      }));

    m_add(parse_string_codes, "%Ah",  alt_is_living,  ({ #'to_string, ({ #'call_other, 'alt, "query_hp", }), }));
    m_add(parse_string_codes, "%Ac",  alt_is_living,  ({ #'to_string, ({ #'call_other, 'alt, "query_cp", }), }));
    m_add(parse_string_codes, "%As",  alt_is_living,  parse_string_codes["%Ac", PSCODES_EVAL_CODE]);  // hysterical raisen.

    parse_string_cache      = ([ ]);
    parse_string_cache[0]   = regreplace(implode(sort_array(m_indices(parse_string_codes), (: strlen($1) < strlen($2) :)), "|"), "\\\\|[$]", "\\\\&", 1);
  }

  if (!member(parse_string_cache, str)) {
    string  format          = "";
    mixed*  args            = ({ });

    foreach (string bit : regexplode(str, parse_string_cache[0])) {
      if (member(parse_string_codes, bit)) {
        format                 += "%s";
        args                   += ({ ({ #'?, parse_string_codes[bit, PSCODES_CHECK_CODE], parse_string_codes[bit, PSCODES_EVAL_CODE], bit, }), });
      }
      else
        format                 += regreplace(bit, "%", "%%", 1);
    }

    parse_string_cache[str] = lambda(({ 'ob, 'alt, }), ({ #'sprintf, format, }) +args);
  //parse_string_cache[str] = sizeof(args) ? lambda(({ 'ob, 'alt, }), ({ #'sprintf, format, }) +args) : sprintf(format);
  }

  return funcall(parse_string_cache[str], ob || previous_object(), alt);
}


////
