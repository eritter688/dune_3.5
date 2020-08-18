/*
**  secure/simul/lordp.c
**
**  status lordp(object ob)
**  Iria@Dune.
**  Ported to DUNE_3.5. - Math
*/
#include <wizlevels.h>
status wizardp (object ob);

int lordp (object ob)
{
  return (wizardp (ob) && ob->query_wiz_level () >= LEVEL_DOMAIN_LORD);
}
