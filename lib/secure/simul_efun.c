#pragma strict_types
#pragma save_types
#pragma no_clone
#pragma no_inherit
#pragma no_shadow

/* protected efuns */
inherit EFUN("livings");

/* mySQL efuns */
#ifdef __MYSQL__
inherit EFUN("mysql");
#endif /* __MYSQL__ */

/* string manipulation functions */
inherit EFUN("ansi");
inherit EFUN("_implode");
inherit EFUN("match_wildcard");
inherit EFUN("parse_time");

/* high-level efuns for livings and users */
inherit EFUN("playerp");

inherit EFUN("wiz_map");
inherit EFUN("wizardp");
inherit EFUN("lordp");
inherit EFUN("baronp");
inherit EFUN("adminp");

/* file/directory/object manipulation efuns */
inherit EFUN("file_time");
inherit EFUN("neat_file");
inherit EFUN("log_file");
inherit EFUN("eval_path");
inherit EFUN("dirname");
inherit EFUN("basename");
inherit EFUN("base_file");
inherit EFUN("place_objects");
inherit EFUN("shadowers");
inherit EFUN("query_heart_beat");


/* array manipulations */
inherit EFUN("sum_array");

/* efuns to display things */
inherit EFUN("tell_others");

/* misc efuns */
inherit EFUN("machinfo");
inherit EFUN("format_time");
inherit EFUN("timezones");
inherit EFUN("snoop");
inherit EFUN("clip");
inherit EFUN("int_to_number");
inherit EFUN("format_number");
inherit EFUN("pickone");
inherit EFUN("getuid");
