/*
**  secure/simul/place_objects.c
**
**  place_objects(string file,int n[,mixed target])
**
**  Clones n maximum instances of the object with filename
**  file into a given target object. If target is omitted,
**  then it is defaulted to the caller object.
**
**  Iria@Dune.
** Copypasted over to Dune_3.5 - Math
*/
string base_file(mixed arg);
object load_object(string str);

varargs int place_objects(string file,int n,mixed target) {
    if(!stringp(file))      raise_error("Bad argument 1 to place_object()\n");
    if(!intp(n) || n < 0)     raise_error("Bad argument 2 to place_object()\n");
    if(!load_object(file))  return 0;
    if(!target)             target = previous_object();
    if(stringp(target))     target = load_object(target);
    if(!objectp(target))    raise_error("Bad argument 3 to place_object()\n");
    if(file[<2..<1] == ".c")  file = file[0..<3];

    if((n -= sizeof(filter(all_inventory(target),
                    (: base_file($1) == $2 :),
                    &file))) <= 0)
      return 0;
    set_this_object(previous_object());
    for(int i = 0; i < n ; i++)
        move_object(clone_object(file), target);
    return n;
}
