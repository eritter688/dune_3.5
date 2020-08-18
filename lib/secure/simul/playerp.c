/*
**  secure/simul/playerp.c
**
**  status playerp(object ob)
**  Iria@Dune.
**  Ported to DUNE_3.5. - Math
*/
#include <object_info.h>

status playerp(object ob) {
  return objectp(ob) && efun::object_info(ob, OI_ONCE_INTERACTIVE);
}
