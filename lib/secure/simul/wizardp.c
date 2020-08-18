/*
**  secure/simul/wizardp.c
**
**  status wizardp(object ob)
**  Iria@Dune.
*/
#include <wizlevels.h>
#include <object_info.h>

int wizardp(object ob) {
  return objectp(ob) && efun::object_info(ob, OI_ONCE_INTERACTIVE) && (ob->query_wiz_level()>=LEVEL_WIZARD);
}
