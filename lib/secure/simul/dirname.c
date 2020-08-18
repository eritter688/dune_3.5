/*
**  secure/simul/dirname.c
**
**  Extracts the dir name from a full path.
**  Iria@Dune.
**  Ported to DUNE_3.5. - Math
*/

string dirname(mixed file) {
    if(objectp(file))
      file = object_name(file);
    return stringp(file) && sprintf("/%s", implode((explode(file,"/")-({ 0, "" }))[0..<2], "/"));
}
