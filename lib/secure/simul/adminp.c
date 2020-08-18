/*
 * Check whether object is at least an Admin level wizard.
 */
#include <wizlevels.h>
#include <object_info.h>

int adminp(object ob) {
  return objectp(ob) && efun::object_info(ob, OI_ONCE_INTERACTIVE) && (ob->query_wiz_level() >= LEVEL_ADMIN);
}
