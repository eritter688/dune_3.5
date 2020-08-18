/*
**  secure/simul/wizardp.c
**
**  status wizardp(object ob)
**  Iria@Dune.
*/
#include <wizlevels.h>
int playerp(object ob);

int wizardp(object ob) {
    return (playerp(ob) && ob->query_wiz_level()>=LEVEL_WIZARD);
}
