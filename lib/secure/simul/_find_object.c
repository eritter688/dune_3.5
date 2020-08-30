/*
**  secure/simul/_find_object.c
**
**  Avanced find_object()
**  Iria@Dune.
**  Ported to DUNE_3.5 unchanged. - Math
*/

string eval_path(string path);
object find_living(string name);

private object find_an_object(string str,object env) {
    object target;
    if(!env) {
        if(str == "me")
                target = this_player();
        else if(str == "here")
                target = environment(this_player());
        else if(target = present(str, this_player()));
        else if(target = present(str, environment(this_player())));
        else if(target = find_living(str));
        else if(target = find_object(eval_path(str)));
        else if(target = (str[0] == '@')
                         && find_object(regreplace(eval_path(str[1..]), "^/", "@", 0)));
        return target;
    }
    if(str == "ENV")
        return environment(env);
    if(str == "ATT")
        return (object)(env->query_attack());
    return present(str,env);
}

object _find_object(string str) {
    if(!str)
        return 0;
    string* bits = map(explode(str,":"), #'trim) - ({ "" });
    int size;
    if(!pointerp(bits) || !(size = sizeof(bits)))
        return 0;
    for(int i = 0; i < size; i++)
        target = find_an_object(bits[i], target);
    return target;
}
