/*
**  secure/simul/eval_path.c
**
**  Evaluate a contracted/relative path to give an absolute path.
**  Note: eval_path returns path with leading '/'.
**  Bubbs@Dune. 7th July '93
**  Iria@Dune. 8th Oct '2000
**
**  Varrack 080501:
**  Setting the environmental variable "completion" will allow
**  eval_path() to search for a path given incomplete identifiers.  It
**  will expand strings in the path argument (trying first for perfect
**  matches) and spit back the unmodified argument if it can't find a
**  match or finds more than one valid match.  Kind of like compleition
**  in UNIX, but suckier.
**
**  Iria@Dune, 29th Aug 2004 : made so eval_path() accepts spaces in
**                             filenames if drivers allows them.
**
** Bubbs, 23/Oct/2009.
** Added <here> to resolve as environment(this_player()).
** Reserved support for further <token> style paths.
**
** Bubbs, 27/Jun/2010.
** Added '=N/' directory support for N'th directory in directory stack.
** Added '=0' is the current directory, '=1' is the top-most directory
** from the user->query_attrib("paths_stack"), etc..
** Added '<direction>' look-up for neighbouring rooms.
**
** Bubbs, 23/Nov/2012.
** Tweaks to stop errors when this_player() == 0.
**
** Bubbs, 1/Apr/2013 (no, seriously).
** Rewrote '=N' support, adding "tagged_dirs" ('=tag') support.
** Rewrote ".." and "..." path resolution, killing 20 year old bug.
**
** Copypasted over to Dune_3.5 - Math 
*/

#include <mudlib.h>

#define EVAL_PATH_WARNING(W)           (this_player() && !(this_player()->query_env_var("brief")) && printf("Path evaluation warning: %s.\n", (W)))

varargs string _implode(mixed *arr,string del,string last);

string eval_path(string path) {
    string *bits  = 0;
    string  pwd   = 0;
    string  rest  = 0;
    int     index = 0;

    !stringp(path) && raise_error("eval_path(): Bad type to argument 1.\n");

#if !defined(__FILENAME_SPACES__)
    path -= " \t";
#endif /* __FILENAME_SPACES__ */
    pwd   = (this_player() && (string)(this_player()->query_path())) || "";

    if (path[0] == '~') {
        if (path == "~")
            path = this_player() ? sprintf("/%s/%s/", WIZ_HOMES, (string)(this_player()->query_real_name())) : "/";
        else if(path[0..1]=="~/")
            path[0..1] = this_player() ? sprintf("/%s/%s/", WIZ_HOMES, (string)(this_player()->query_real_name())) : "/";
        else
            path[0..0] = "/" +WIZ_HOMES +"/";
    }
    else if (path[0..1] == "%/") path[0..1] = "/obj/";
    else if (path[0..1] == "#/") path[0..1] = "/room/";
    else if (path[0..1] == "&/") path[0..1] = "/obj/cmds/";
    else if (path[0..1] == "$/") path[0..1] = "/secure/";
#if 0
    else if ((path == "=0") || (path[0..2] == "=0/"))
        path = strlen(pwd) ? sprintf("/%s%s", pwd, path[2..]) : sprintf("/%s", path[2..]);
    else if (this_player() && (sscanf(path, "=%U%s", index, rest) == 2)) {
        bits = (string*)(this_player()->query_attribute("paths_stack"));
        if (pointerp(bits) && (index <= sizeof(bits)) && ((rest == "") || (rest[0] == '/')))
            path = sprintf("/%s%s", bits[index -1], rest);
        else
            EVAL_PATH_WARNING("Directory '=" +index +"' not found");
    }
#else
    else if ((path[0] == '=') && this_player()) {
        string dir = regreplace(path, "^=([a-zA-Z0-9_]*)(/.*|)$", "\\1", 1);
        rest = path[1 +strlen(dir)..];
        if (dir[0] == '=')
            EVAL_PATH_WARNING("Invalid directory '" +path +"'");
#if 0
        else if (dir == "")
            EVAL_PATH_WARNING("Missing directory");
#endif
        else if (sscanf(dir, "%~+U%~+0s") == 2) {
            index  = to_int(dir);
            bits   = index && (string*)(this_player()->query_attribute("paths_stack"));
            dir    = index ? (pointerp(bits) && (index <= sizeof(bits)) && bits[index -1]) : (pwd || "");
            if (dir)
                path = sprintf("/%s%s", dir, rest);
            else
                EVAL_PATH_WARNING("Stack directory '=" +dir +"' not found");
        }
        else if (!sscanf(dir, "%~U")) {
            string tagged = (string)(this_player()->query_env_var("tagged_dirs"));
            bits   = stringp(tagged) && explode(regreplace(tagged, "[ \t,;:|]+", " ", 1), " ");
            bits &&= bits -filter(bits, #'strstr, dir +"=");
            if (sizeof(bits))
                path = eval_path(bits[0][strlen(dir) +1..]) +rest;
            else
                EVAL_PATH_WARNING("Tagged directory '=" +dir +"' not found");
        }
        else
            EVAL_PATH_WARNING("Directory '=" +dir +"' not found");
    }
#endif
    else if (this_player() && (path[0] == '<') && (path[<1] == '>')) {
        object ob    = environment(this_player());
        string path2 = 0;
        if (!ob);
        else if (path == "<here>")
            path2 = (load_name(ob) +".c");
        else if (path == "<heredir>")
            path2 = regreplace(load_name(ob), "[^/]*$", "", 0);
        else if (path[..1] == "<@") {
            object* obs = filter_objects(users(), "id", path[2..<2]);
            obs         = sizeof(obs) ? obs : funcall(symbol_function("_find_object"), path[2..<2]);
            path2       = sizeof(obs) && load_name(environment(obs[0]));
            path2     &&= path2 +".c";
        }
        else {
            switch(path) {
                case "<n>" :  path2 = "north";      break;
                case "<nw>":  path2 = "northwest";  break;
                case "<w>" :  path2 = "west";       break;
                case "<sw>":  path2 = "southwest";  break;
                case "<s>" :  path2 = "south";      break;
                case "<se>":  path2 = "southeast";  break;
                case "<e>" :  path2 = "east";       break;
                case "<ne>":  path2 = "northeast";  break;
                case "<u>" :  path2 = "up";         break;
                case "<d>" :  path2 = "down";       break;
                default    :  path2 = path[1..<2];  break;
            }
            path2   = (string)(ob->query_exit(path2));
            path2 &&= ((path[<2..] == ".c") ? path2 : (path2 +".c"));
        }
        path = path2 ? (((path2[0] == '/') ? "" : "/") +path2) : path;
    }
    else if(path[0] != '/')
        path = strlen(pwd) ? sprintf("/%s/%s", pwd, path) : sprintf("/%s", path);

#if 0
    bits = filter(explode(path, "/"), #'!=, ".");
    while((index = member(bits, "..")) != -1)
        switch(index) {
            case  0:  bits = bits[1..];                            break;
            default:  bits = bits[0..index -2] +bits[index +1..];  break;
        }
    while((index = member(bits, "...")) != -1)
        switch(index) {
            case  0:  bits = bits[1..];                            break;
            case  1:  bits = bits[2..index -1];                    break;
            default:  bits = bits[0..index -3] +bits[index +1..];  break;
        }
#else
    // Bubbs, 01/Apr/2013 (no, seriously).  The above resolution algorithm
    // is the original, 20 years old.   Although I think there may have a
    // bug (when mixing multiple ".." and "..."s, e.g. "...\.."), I'm loathed
    // to change it without thinking thoroughly on the topic.

    bits  = explode(path, "/") -({ ".", });
    index = 0;
    while(index != -1) {
#if 0
        string* dots = bits & ({ ".", "..", "...", });
        index = sizeof(dots) ? min(map(dots, (: member($2, $1) :), bits)) : -1;
#else
        int index1 = member(bits, "..");
        int index2 = member(bits, "...");
        index = (index1 == -1) ? index2 : (index2 == -1) ? index1 : min(index1, index2);
#endif
        switch(index) {
            case -1:  /* ignored. */                                                break;
            case  0:  bits = bits[1..];                                             break;
            case  1:  bits = bits[2..];                                             break;
            default:  bits = bits[..index -strlen(bits[index])] +bits[index +1..];  break;
        }
    }
#endif

    index = sizeof(bits) -1;
    if ((index != -1) && (bits[index] == ""))
        bits = bits[0..index -1];

#if defined(DUNE)
    /* Start 'completion' code */
    if (this_player() && (this_player()->query_env_var("completion"))) {
        string *files = 0;
        int     size  = sizeof(bits);
        for(index = 1; index < size; index++) {
            if (strstr(bits[index], "*") != -1)
                break;
            path  = sprintf("%s/", implode(bits[0..index -1], "/"));
            files = get_dir(path, 1);
            if (!pointerp(files))        // Iria: regexp() causes a bug when cannot read dir
                continue;
            files = regexp(files, sprintf("^%s", bits[index]));
            if (member(files, bits[index]) > -1)
                continue;
            if (sizeof(files) > 1) {
                EVAL_PATH_WARNING("Ambiguous completion (" +_implode(files, ", ", " or ") +" ?)");
                break;
            }
            if (sizeof(files) == 1)
                bits[index] = files[0];
        }
    }
#endif /* DUNE */

    path = implode(bits, "/");
    if (!strlen(path))
      path = "/";

    return path;
}
