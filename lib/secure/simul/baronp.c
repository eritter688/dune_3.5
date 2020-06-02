/*
 * Check whether object is at least a Baron level wizard.
 */
#include <wizlevels.h>

int wizardp(object ob);

int baronp(object ob) {
    return ((wizardp(ob)) &&
            (ob->query_wiz_level() >= LEVEL_BARON));
}