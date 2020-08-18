/*
 * Check whether object is at least a Baron level wizard.
 */
#include <wizlevels.h>
#include <object_info.h>

int baronp(object ob) {
  return objectp(ob) && efun::object_info(ob, OI_ONCE_INTERACTIVE) && (ob->query_wiz_level() >= LEVEL_BARON);
}
