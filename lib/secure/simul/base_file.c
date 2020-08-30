/*
**  secure/simul/base_file.c
**
**  Strips the #<refcount> from the object's file name,
**  also strip (xxx) from virtual objects file name.
**  string base_file(string file);
**  string base_file(object ob);
**
**  Iria@Dune.
**  Ported to DUNE_3.5, now returns with leading / intact. - Math
*/

string base_file(mixed arg) {
    if(objectp(arg))
      arg = object_name(arg);
    if(!stringp(arg))
      raise_error("Bad type to argument 1 to base_file()\n");
    if(sscanf(arg,"%s#%~d", arg) != 2)
      sscanf(arg,"%s(%~s)", arg);
    if(arg[0] == '@')  arg = arg[1..];
return arg;
}
