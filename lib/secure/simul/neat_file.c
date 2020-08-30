/*
**  secure/simul/neat_file.c
**
**  string neat_file(string file);
**  string neat_file(object ob);
**  Iria@Dune.
*/

string neat_file(mixed file) {
    if(!file || (!stringp(file) && !objectp(file)))
        raise_error("Bad argument 1 to neat_file()\n");

    string tmp=stringp(file) ? file : object_name(file);
    string tmp2;

    if(tmp[0] != '/' && tmp[0] != '@')         tmp = sprintf("/%s", tmp);
    if(sscanf(tmp,"/"WIZ_HOMES"/%s", tmp2))    tmp = sprintf("~%s", tmp2);
    else if(sscanf(tmp, "/obj/cmds/%s", tmp2)) tmp = sprintf("&/%s", tmp2);
    else if(sscanf(tmp, "/obj/%s", tmp2))      tmp = sprintf("%%/%s", tmp2);
    else if(sscanf(tmp, "/room/%s", tmp2))     tmp = sprintf("#/%s", tmp2);
    else if(sscanf(tmp, "/secure/%s", tmp2))   tmp = sprintf("$/%s", tmp2);
    return tmp;
}
