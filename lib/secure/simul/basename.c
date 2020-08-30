/*
**  secure/simul/basename.c
**
**  Strips a full path to the a file name.
**  Iria@Dune.
**  Ported to DUNE_3.5. - Math
*/

string basename(mixed file) {
    string *subs;
    if(objectp(file))
      file = object_name(file);
    return stringp(file)
           && (sizeof(subs = explode(file, "/")-({0, ""})) == 0 ? "/" : subs[<1]);
}
