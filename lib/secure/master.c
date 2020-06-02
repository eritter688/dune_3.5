/*
 * Master Object for Dune MUD.
 *
 * The master object is assembled from the included files below.
 *
 * About the master object:
 *
 * The master is the gateway between the interpreter and the mudlib to perform
 * actions with mudlib specific effects. Calls to the master by the interpreter
 * have an automatic catch(E) in effect. Functions to be called by the driver
 * can be public or static, but not private.
 *
 * Note that the master is loaded first of all objects. Thus you shouldn’t
 * inherit an other object, nor is the compiler able to search include files
 * (read: they must be specified with full path).
 *
 * Further information:
 *
 * http://abathur.github.io/ldmud-doc/build/html/master/overview.html
 */

// Definitions //

#include "master.ob/defs.h"

// Prototypes //

#include "master.ob/proto.h"

// Implementation Files //

#include "master.ob/master_init.c"
#include "master.ob/connect.c"
#include "master.ob/runtime.c"
#include "master.ob/error.c"
#include "master.ob/permissions.c"
#include "master.ob/valid.c"
#include "master.ob/ed.c"
