/*
**  secure/simul/lordp.c
**
**  status lordp(object ob)
**  Iria@Dune.
**  Ported to DUNE_3.5. - Math
*/
#include <wizlevels.h>
#include <object_info.h>

int lordp(object ob) {
  return objectp(ob)
         && efun::object_info(ob, OI_ONCE_INTERACTIVE)
         && (ob->query_wiz_level () >= LEVEL_DOMAIN_LORD);
}
