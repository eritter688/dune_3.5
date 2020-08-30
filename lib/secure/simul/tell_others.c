/*
**  secure/simul/tell_others.c
**
**  Similar to tell_room() but uses parse_string().
**  Iria@Dune.
**
** Bubbs@Dune, 2017-01-29: Replaced lambda()s with inline closures.
** Copied over to Dune 3.5 - Math
*/
        status playerp(object ob);
varargs string parse_string(string str,object ob);
void    tell_object(object ob,string str);

private closure tell_them,
                filter_npcs;

varargs void tell_others(object teller,string message,int opt,mixed exclude) {
    object *us,room;

    if(!objectp(teller))                raise_error("Bad argument 1 to tell_others()\n");
    if(!stringp(message))               raise_error("Bad argument 2 to tell_others()\n");
    if(!intp(opt))                      raise_error("Bad argument 3 to tell_others()\n");
    if(exclude && (!objectp(exclude)
               && !pointerp(exclude)))  raise_error("Bad argument 4 to tell_others()\n");
    if(!(room=environment(teller)))     return;

    if(!closurep(tell_them))
        tell_them = (: tell_object($1, parse_string($3, $2, $1)) :);
//        tell_them=lambda(
//            ({ 'ob, 'teller, 'msg }),
//            ({ #'tell_object, 'ob, ({ #'parse_string, 'msg, 'teller, 'ob }) })
//        );
    if(!closurep(filter_npcs))
        filter_npcs = (: living($1) && !playerp($1) :);
//        filter_npcs=lambda(
//            ({ 'ob }),
//            ({ #'&&, ({ #'living, 'ob }), ({ #'!, ({ #'playerp, 'ob }) }) })
//        );

    switch(opt) {
        default:
        case 0:     us=filter(all_inventory(room),#'playerp);       break;
        case 1:     us=filter(all_inventory(room),filter_npcs);     break;
        case 2:     us=filter(all_inventory(room),#'living);        break;
        case 3:     us=all_inventory(room);                         break;
    }
    us -=({ teller });
    if(objectp(exclude))        us -=({ exclude });
    else if(pointerp(exclude))  us -=exclude;
    map(us,tell_them,teller,&message);
}
