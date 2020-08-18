/*
 * Check whether object is at least an Admin level wizard.
 */
#include <wizlevels.h>

int wizardp(object ob);

int adminp(object ob) {
    return ((wizardp(ob)) && (ob->query_wiz_level() >= LEVEL_ADMIN));
}
