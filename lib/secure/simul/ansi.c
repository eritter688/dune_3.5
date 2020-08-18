/*
**  secure/simul/ansi.c
**
**  Original: Bubbs@TW.
**  Ported to Dune and enhanced: Iria@Dune.
**    added query_ansi() & recoded ansi_colour(): Iria@Dune.
**    added ansi_cursor()                       : Iria@Dune.
**    added color aliases "=<ansi combination>" : Iria@Dune.
**    added parse_ansi()                        : Iria@Dune.
**      improved to use regreplace              : Tzar@Dune - Dec 2004.
**    added remove_ansi_tokens()                : Iria@Dune
**      improved to use regreplace              : Tzar@Dune - Dec 2004.
**    added query_colors()                      : Iria@Dune.
**    added valid_color()                       : Iria@Dune.
**    added query_color_setting()               : Iria@Dune.
**    added set_color_setting()                 : Iria@Dune.
**    added remove_color_setting()              : Iria@Dune.
**    added query_ansi_colors()                 : Iria@Dune.
**    added query_ansi_modifiers()              : Iria@Dune.
**    added random_color()                      : Iria@Dune code by ???
** Copypasted over to Dune_3.5 - Math
*/
#define ANSI_CURSOR     0x01
#define ANSI_COLOR      0x02
#define ANSI_MODIFIER   0x04

static mapping ansi_codes;
static closure remove_ansi_codes;

        string subst(string str,string tok,string repl);
        status playerp(object ob);
        string remove_ansi_tokens(string str);

static private void check_ansi_codes() {
    if(ansi_codes) return;
    ansi_codes = ([
        "home"     : "\e[H"          ; ANSI_CURSOR,
        "save"     : "\e7"           ; ANSI_CURSOR,
        "restore"  : "\e8"           ; ANSI_CURSOR,
        "standard" : "\e[2;37;0m"    ; ANSI_MODIFIER,
        "normal"   : "\e[0m"         ; ANSI_MODIFIER,
        "off"      : "=normal"       ; ANSI_MODIFIER,
        "bold"     : "\e[1m"         ; ANSI_MODIFIER,
        "highlight": "=bold"         ; ANSI_MODIFIER,
        "underline": "\e[4m"         ; ANSI_MODIFIER,
        "flash"    : "\e[5m"         ; ANSI_MODIFIER,
        "blink"    : "=flash"        ; ANSI_MODIFIER,
        "inverse"  : "\e[7m"         ; ANSI_MODIFIER,
        "black"    : "\e[30m"        ; ANSI_COLOR,
        "red"      : "\e[31m"        ; ANSI_COLOR,
        "green"    : "\e[32m"        ; ANSI_COLOR,
        "yellow"   : "\e[1;33m"      ; ANSI_COLOR,
        "brown"    : "\e[33m"        ; ANSI_COLOR,
        "blue"     : "\e[34m"        ; ANSI_COLOR,
        "magenta"  : "\e[35m"        ; ANSI_COLOR,
        "purple"   : "=magenta"      ; ANSI_COLOR,
        "pink"     : "=bold magenta" ; ANSI_COLOR,
        "cyan"     : "\e[36m"        ; ANSI_COLOR,
        "white"    : "\e[37m"        ; ANSI_COLOR,
        "bgblack"  : "\e[40m"        ; ANSI_COLOR,
        "bgred"    : "\e[41m"        ; ANSI_COLOR,
        "bggreen"  : "\e[42m"        ; ANSI_COLOR,
        "bgbrown"  : "\e[43m"        ; ANSI_COLOR,
        "bgyellow" : "\e[43m"        ; ANSI_COLOR,
        "bgblue"   : "\e[44m"        ; ANSI_COLOR,
        "bgmagenta": "\e[45m"        ; ANSI_COLOR,
        "bgcyan"   : "\e[46m"        ; ANSI_COLOR,
        "bgwhite"  : "\e[47m"        ; ANSI_COLOR,
    ]);
}

status query_ansi(object ob) {
    if(!ob || !playerp(ob)) return 0;
    return (ob->defined_env_var("ansi") && (ob->query_env_var("ansi") != "off"));
}

string *query_ansi_colors() {
    check_ansi_codes();
    return filter(
        m_indices(ansi_codes),
      //lambda( ({ 'col }), ({ #'==, ({ #'[, ansi_codes, 'col, 1 }), ANSI_COLOR }) ));
        (: ansi_codes[$1, 1] == ANSI_COLOR :));
}

string *query_ansi_modifiers() {
    check_ansi_codes();
    return filter(
        m_indices(ansi_codes),
      //lambda( ({ 'col }), ({ #'==, ({ #'[, ansi_codes, 'col, 1 }), ANSI_MODIFIER }) ));
        (: ansi_codes[$1, 1] == ANSI_MODIFIER :));
}

varargs string cursor_up(int number, object target) {
    check_ansi_codes();
    if (!target) target = this_player();
    return (query_ansi(target) ? sprintf("\e[%dA", number) : "");
}

varargs string cursor_down(int number, object target) {
    check_ansi_codes();
    if (!target) target = this_player();
    return (query_ansi(target) ? sprintf("\e[%dB", number) : "");
}

varargs string cursor_right(int number, object target) {
    check_ansi_codes();
    if (!target) target = this_player();
    return (query_ansi(target) ? sprintf("\e[%dC", number) : "");
}

varargs string cursor_left(int number, object target) {
    check_ansi_codes();
    if (!target) target = this_player();
    return (query_ansi(target) ? sprintf("\e[%dD", number) : "");
}

varargs string cursor_position(int l, int c, object target) {
    check_ansi_codes();
    if (!target) target = this_player();
    return (query_ansi(target) ? sprintf("\e[%d;%dH", l, c) : "");
}

varargs string erase_line(int mode, object target) {
    check_ansi_codes();
    if (!target) target = this_player();
    return (query_ansi(target) ? sprintf("\e[%dK", mode) : "");
}

varargs string erase_screen(int mode, object target) {
    check_ansi_codes();
    if (!target) target = this_player();
    return (query_ansi(target) ? sprintf("\e[%dJ", mode) : "");
}

varargs string ansi_cursor(string code, object target) {
    if (!stringp(code))
        raise_error("Bad argument 1 to ansi_cursor()\n");
    if (!target) target = this_player();
    if (!query_ansi(target)) return "";
    check_ansi_codes();
    return ((!member(ansi_codes, code) || ansi_codes[code, 1]) ? "" : funcall(ansi_codes[code, 0], code, target));
}

varargs string ansi_colour(string code, object target) {
    if (!stringp(code))
#ifdef ANSI_COLOUR_CAN_BREAK
        raise_error("Bad argument 1 to ansi_colour()\n");
#else
        return "";
#endif // ANSI_COLOUR_CAN_BREAK
    if (!target)             target = this_player();
    if (!query_ansi(target)) return "";
    check_ansi_codes();
    string colour = "";
    foreach (string col : explode(lower_case(code), " ")) {
      //if (member(ansi_codes, col) && ansi_codes[col, 1])
      //  colour += (ansi_codes[col, 0][0] == '=') ? ansi_colour(ansi_codes[col, 0][1..], target) : ansi_codes[col, 0];
        mixed colour_code = ansi_codes[col, 1] && funcall(ansi_codes[col, 0], col, target);
        if (!colour_code && strlen(col) && !strlen(regreplace(col, "^(bg[h#]|[h#]bg|[h#])([0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f])$", "", 0)))
            colour_code = ((member(col, 'g') == -1) ? "\e[38;2;" : "\e[48;2;") +to_int("0x" +col[<6..<5]) +";" +to_int("0x" +col[<4..<3]) +";" +to_int("0x" +col[<2..]) +"m";
        if (stringp(colour_code))
            colour += (colour_code[0] == '=') ? ansi_colour(colour_code[1..], target) : colour_code;
    }
    return colour;
}

varargs string parse_ansi(string str, object ob) {
    string rstring;
    if (!stringp(str))
        raise_error("Bad argument 1 to parse_ansi()\n");
    if (!ob)
        ob = this_player();
#ifdef ANSI_COLOUR_CAN_BREAK
    if (!objectp(ob))
        raise_error("Bad argument 2 to parse_ansi()\n");
#endif // ANSI_COLOUR_CAN_BREAK
    if (!playerp(ob) || !ob->defined_env_var("ansi") || (ob->query_env_var("ansi") == "off"))
        return remove_ansi_tokens(str);
#if 1
  //string  regex   = "[$](OFF|CLEAR|(NO|)(BOLD|UNDERL|INVERSE|BLINK)|(BG|)(BLACK|RED|GREEN|BROWN|YELLOW|BLUE|PINK|PURPLE|MAGENTA|CYAN|WHITE))";
  //string  regex   = "[$](OFF|CLEAR|(NO|)(BOLD|UNDERL|INVERSE|BLINK)|(BG|)(BLACK|RED|GREEN|BROWN|YELLOW|BLUE|PINK|PURPLE|MAGENTA|CYAN|WHITE)|X(BG|)((CUBE|C)([0-5][0-5][0-5])|(GR[AE]Y|G)([0-9][0-9]|[0-9])|([0-9][0-9][0-9]|[0-9][0-9]|[0-9])))";
    string  regex   = "[$](OFF|CLEAR|(NO|)(BOLD|UNDERL|INVERSE|BLINK)|(BG|)(BLACK|RED|GREEN|BROWN|YELLOW|BLUE|PINK|PURPLE|MAGENTA|CYAN|WHITE)|X(BG|)((CUBE|C)([0-5][0-5][0-5])|(GR[AE]Y|G)([0-9][0-9]|[0-9])|([0-9][0-9][0-9]|[0-9][0-9]|[0-9]))|[H#](BG|)([0-9A-F][0-9A-F][0-9A-F][0-9A-F][0-9A-F][0-9A-F]))";
    closure replace =
        (:
            switch ($1) {
            case "$OFF"      :  return "\e[0m";
            case "$BOLD"     :  return "\e[1m";
            case "$UNDERL"   :  return "\e[4m";
            case "$INVERSE"  :  return "\e[7m";
            case "$NOBOLD"   :  return "\e[22m";
            case "$NOUNDERL" :  return "\e[24m";
            case "$NOINVERSE":  return "\e[27m";
            case "$NOBLINK"  :  return "\e[25m";
            case "$BLINK"    :  return "\e[5m";
            case "$BLACK"    :  return "\e[30m";
            case "$RED"      :  return "\e[31m";
            case "$GREEN"    :  return "\e[32m";
            case "$BROWN"    :  return "\e[33m";
            case "$YELLOW"   :  return "\e[33;1m";
            case "$BLUE"     :  return "\e[34m";
            case "$PINK"     :  return "\e[35m";
            case "$PURPLE"   :  return "\e[35;1m";
            case "$MAGENTA"  :  return "\e[35m";
            case "$CYAN"     :  return "\e[36m";
            case "$WHITE"    :  return "\e[37m";
            case "$BGBLACK"  :  return "\e[40m";
            case "$BGRED"    :  return "\e[41m";
            case "$BGGREEN"  :  return "\e[42m";
            case "$BGBROWN"  :  return "\e[43m";
            case "$BGBLUE"   :  return "\e[44m";
            case "$BGPINK"   :  return "\e[45m";
            case "$BGMAGENTA":  return "\e[45m";
            case "$BGCYAN"   :  return "\e[46m";
            case "$BGWHITE"  :  return "\e[47m";
            case "$CLEAR"    :  return "\e[2J\e[H";
            }
            if (($1[..1] == "$H") || ($1[..1] == "$#"))
                return (($1[2..3] == "BG") ? "\e[48;2;" : "\e[38;2;") +to_int("0x" +$1[<6..<5]) +";" +to_int("0x" +$1[<4..<3]) +";" +to_int("0x" +$1[<2..]) +"m";
            if ($1[..1] != "$X")
                return $1;
            if (regreplace($1, "^[$]X(BG|)(CUBE|C)([0-5][0-5][0-5])$", "", 0) == "")
                return (($1[..3] == "$XBG") ? "\e[48;5;" : "\e[38;5;") +min(16 +(36 *to_int($1[<3..<3])) +(6 *to_int($1[<2..<2])) +to_int($1[<1..<1]), 255) +"m";
            if (regreplace($1, "^[$]X(BG|)(GR[AE]Y|G)([0-9][0-9]|[0-9])$", "", 0) == "")
                return (($1[..3] == "$XBG") ? "\e[48;5;" : "\e[38;5;") +min(232 +to_int($1 & "0123456789"), 255) +"m";
            if (regreplace($1, "^[$]X(BG|)([0-9][0-9][0-9]|[0-9][0-9]|[0-9])$", "", 0) == "")
                return (($1[..3] == "$XBG") ? "\e[48;5;" : "\e[38;5;") +min(to_int($1 & "0123456789"), 255) +"m";
            return $1;
        :);

    return regreplace(str, regex, replace, 1);
#else
    rstring = regreplace(str,     "[$]OFF",      "\e[0m",      1);
    rstring = regreplace(rstring, "[$]BOLD",     "\e[1m",      1);
    rstring = regreplace(rstring, "[$]UNDERL",   "\e[4m",      1);
    rstring = regreplace(rstring, "[$]INVERSE",  "\e[7m",      1);
    rstring = regreplace(rstring, "[$]BLINK",    "\e[5m",      1);
    rstring = regreplace(rstring, "[$]BLACK",    "\e[30m",     1);
    rstring = regreplace(rstring, "[$]RED",      "\e[31m",     1);
    rstring = regreplace(rstring, "[$]GREEN",    "\e[32m",     1);
    rstring = regreplace(rstring, "[$]BROWN",    "\e[33m",     1);
    rstring = regreplace(rstring, "[$]YELLOW",   "\e[1;33m",   1);
    rstring = regreplace(rstring, "[$]BLUE",     "\e[34m",     1);
    rstring = regreplace(rstring, "[$]PINK",     "\e[1;35m",   1);
    rstring = regreplace(rstring, "[$]PURPLE",   "\e[35m",     1);
    rstring = regreplace(rstring, "[$]MAGENTA",  "\e[35m",     1);
    rstring = regreplace(rstring, "[$]CYAN",     "\e[36m",     1);
    rstring = regreplace(rstring, "[$]WHITE",    "\e[37m",     1);
    rstring = regreplace(rstring, "[$]CLEAR",    "\e[2J\e[H",  1);
    return rstring;
#endif
}

status valid_color(string color) {
    return (stringp(color) && (member(ansi_codes, color) && (ansi_codes[color, 1] != ANSI_CURSOR)));
}

varargs string query_color_setting(string pref, object pl, string var) {
    string *cols;
    if (!objectp(pl))
        pl = this_player();
    if (!stringp(pref))  raise_error("Bad argument 1 to query_color_setting()\n");
    if (!objectp(pl))    raise_error("Bad argument 2 to query_color_setting()\n");
    if (!stringp(var))   var = "color";

    if (!playerp(pl) || !pl->defined_env_var(var))
        return "normal";
    cols = regexp(map(explode(pl->query_env_var(var), ","), #'trim), sprintf("^%s:", pref));
    return (!sizeof(cols) || (sscanf(cols[0], "%~s:%s", pref) != 2)) ? "normal" : trim(pref);
}

varargs void set_color_setting(string pref, string color, object pl, string var) {
    string *cols;
    if (!objectp(pl))
        pl = this_player();
    if (!stringp(pref))  raise_error("Bad argument 1 to set_color_setting()\n");
    if (!stringp(color)) raise_error("Bad argument 2 to set_color_setting()\n");
    if (!objectp(pl))    raise_error("Bad argument 3 to set_color_setting()\n");
    if (!playerp(pl))    return;
    if (!stringp(var))   var = "color";

    if (!pl->defined_env_var(var))
        return pl->set_env_var(var, sprintf("%s:%s", pref, color));
    cols = map(explode(pl->query_env_var(var), ","), #'trim);
    cols -= regexp(cols, sprintf("^%s:", pref));
    cols += ({ sprintf("%s:%s", pref,color), });
    pl->set_env_var(var, implode(cols, ", "));
}

varargs void remove_color_setting(string pref, object pl, string var) {
    string *cols;
    if (!pl)
        pl = this_player();
    if (!stringp(pref))  raise_error("Bad argument 1 to remove_color_setting()\n");
    if (!objectp(pl))    raise_error("Bad argument 2 to remove_color_setting()\n");
    if (!stringp(var))   var = "color";

    if (!playerp(pl) || !pl->defined_env_var(var))
        return;
    cols = map(explode(pl->query_env_var(var), ","), #'trim);
    cols -= regexp(cols, sprintf("^%s:", pref));
    if (!sizeof(cols))    pl->delete_env_var(var);
    else                 pl->set_env_var(var, implode(cols, ", "));
}

string remove_ansi(string astr) {
    if(!stringp(astr))
        raise_error("Bad argument 1 to remove_ansi()\n");
  //return regreplace(astr, "\e\\[[0-9]+(;[0-9]+)*m", "", 1);
    return regreplace(astr, "\e(\\[|\\])[0-9;]*m", "", 1);
}

string remove_ansi_tokens(string str) {
    if (!stringp(str))
        raise_error("Bad argument 1 to remove_ansi_tokens()\n");
    return regreplace(
        str,
      //"[$](OFF|BOLD|UNDERL|INVERSE|BLINK|BLACK|RED|GREEN|BLUE|BROWN|YELLOW|PINK|PURPLE|CYAN|MAGENTA|WHITE)","",1
      //"[$](OFF|(NO|)(BOLD|UNDERL|INVERSE|BLINK)|(BG|)(BLACK|RED|GREEN|BROWN|YELLOW|BLUE|PINK|PURPLE|MAGENTA|CYAN|WHITE))","",1
      //"[$](OFF|(NO|)(BOLD|UNDERL|INVERSE|BLINK)|(BG|)(BLACK|RED|GREEN|BROWN|YELLOW|BLUE|PINK|PURPLE|MAGENTA|CYAN|WHITE)|X(BG|)((CUBE|C)([0-5][0-5][0-5])|(GR[AE]Y|G)([0-9][0-9]|[0-9])|([0-9][0-9][0-9]|[0-9][0-9]|[0-9])))","",1
        "[$](OFF|(NO|)(BOLD|UNDERL|INVERSE|BLINK)|(BG|)(BLACK|RED|GREEN|BROWN|YELLOW|BLUE|PINK|PURPLE|MAGENTA|CYAN|WHITE)|X(BG|)((CUBE|C)([0-5][0-5][0-5])|(GR[AE]Y|G)([0-9][0-9]|[0-9])|([0-9][0-9][0-9]|[0-9][0-9]|[0-9]))|[H#](BG|)([0-9A-F][0-9A-F][0-9A-F][0-9A-F][0-9A-F][0-9A-F]))","",1
    );
}

string random_color() {
    string *all_colors;
    all_colors = ({
        ansi_colour("red"),
        ansi_colour("inverse"),
        ansi_colour("bold"),
        ansi_colour("underline"),
        ansi_colour("green"),
        ansi_colour("blue"),
        ansi_colour("brown"),
        ansi_colour("purple"),
        ansi_colour("pink"),
        ansi_colour("cyan"),
        ansi_colour("white"),
        ansi_colour("blink"),
    });
    return all_colors[random(sizeof(all_colors))];
}

/*
** Kept for compatibility
** We should removed them as soon as possible.
** Iria@Dune.
*/
string highlight(string str) { return sprintf("%s%s%s", ansi_colour("bold"),  str || "", ansi_colour("normal")); }
string to_purple(string str) { return sprintf("%s%s%s", ansi_colour("blue"),  str || "", ansi_colour("normal")); }
string to_green(string str)  { return sprintf("%s%s%s", ansi_colour("green"), str || "", ansi_colour("normal")); }
string to_red(string str)    { return sprintf("%s%s%s", ansi_colour("red"),   str || "", ansi_colour("normal")); }
string to_blue(string str)   { return sprintf("%s%s%s", ansi_colour("blue"),  str || "", ansi_colour("normal")); }
