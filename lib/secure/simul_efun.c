#pragma strict_types
#pragma save_types

/* protected efuns */
inherit EFUN("livings");
inherit EFUN("destruct");
inherit EFUN("remove_interactive");
inherit EFUN("command");
inherit EFUN("shutdown");

/* mySQL efuns */
#ifdef __MYSQL__
inherit EFUN("mysql");
#endif /* __MYSQL__ */

/* general purpose efuns */
inherit EFUN("stdlib");
inherit EFUN("notify_fail");
// inherit EFUN("getopt");

/* string manipulation functions */
inherit EFUN("ansi");
inherit EFUN("trimstr");
inherit EFUN("subst");
inherit EFUN("_implode");
inherit EFUN("match_wildcard");
inherit EFUN("parse_string");
inherit EFUN("parse_time");
inherit EFUN("capitalize_words");
inherit EFUN("pad");
inherit EFUN("fwrite");
inherit EFUN("misc_strings");

/* high-level efuns for livings and users */
inherit EFUN("invisp");
inherit EFUN("playerp");
inherit EFUN("guestp");
inherit EFUN("testerp");
inherit EFUN("builderp");
inherit EFUN("wiz_map");
inherit EFUN("wizardp");
inherit EFUN("slordp");
inherit EFUN("lordp");
inherit EFUN("baronp");
inherit EFUN("adminp");
inherit EFUN("user_exists");
inherit EFUN("update_actions");
inherit EFUN("query_prompt");
inherit EFUN("roomp");

/* file/directory/object manipulation efuns */
inherit EFUN("file_time");
inherit EFUN("neat_file");
inherit EFUN("log_file");
inherit EFUN("eval_path");
inherit EFUN("file_c");
inherit EFUN("dirname");
inherit EFUN("basename");
inherit EFUN("base_file");
inherit EFUN("grepf");
inherit EFUN("crc");

inherit EFUN("find_items");
inherit EFUN("_find_object");
inherit EFUN("deep_present");
inherit EFUN("load_object");
inherit EFUN("place_objects");
#if !__EFUN_DEFINED__(all_environment)
inherit EFUN("all_environment");
#endif /* all_environment */

inherit EFUN("shadowers");
#if !__EFUN_DEFINED__(clonep)
inherit EFUN("clonep");
#endif /* clonep */
inherit EFUN("query_heart_beat");

inherit EFUN("map_to_string");
inherit EFUN("array_to_string");
inherit EFUN("word_wrap");

/* mapping manipulations */
inherit EFUN("expand_mapping");

/* array manipulations */
inherit EFUN("sum_array");
inherit EFUN("set_of");
inherit EFUN("exclude_array");
inherit EFUN("exclude_element");

/* efuns to display things */
inherit EFUN("tell_object");
inherit EFUN("tell_others");
inherit EFUN("shout");

/* misc efuns */
inherit EFUN("machinfo");
inherit EFUN("format_time");
inherit EFUN("timezones");
inherit EFUN("snoop");
inherit EFUN("article");
inherit EFUN("grammar");
inherit EFUN("roll_dice");
inherit EFUN("localcmd");
inherit EFUN("clip");
inherit EFUN("math");
inherit EFUN("int_to_number");
inherit EFUN("format_number");
inherit EFUN("convert");
inherit EFUN("pickone");
//inherit EFUN("reverse");

//inherit EFUN("creator");
inherit EFUN("getuid");

inherit EFUN("wizlist");
//inherit EFUN("add_worth");  // file does nothing?

inherit EFUN("efun308");            // efun308()/set_environment() & set_this_player()
inherit EFUN("set_this_player");    // are disabled for security reasons

/* Dune specific efuns */
inherit EFUN("extra/sound_funs");   // ZMUD sound extensions

inherit EFUN("query_idle");

/* Some set efuns */
inherit EFUN("intersect");
inherit EFUN("union");

inherit EFUN("listen_in_room");
