/*
**  secure/simul/playerp.c
**
**  status playerp(object ob)
**  Iria@Dune.
**  Ported to DUNE_3.5. - Math
*/
status playerp(object ob) {
    return objectp(ob) && query_once_interactive(ob);
}
