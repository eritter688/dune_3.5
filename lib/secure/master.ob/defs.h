/*
 * Master Object Definitions
 *
 */
#ifndef _H_MASTER_DEFS_H_
#define _H_MASTER_DEFS_H_

// additional fortification for master object
#pragma strong_types
#pragma pedantic

// file parsed on startup and shutdown
#define INIT_FILE                          ""
#define SHUT_FILE                          ""

#define OBJECT_CREATE_FUNCTION_NAME        "create"
#define OBJECT_RESET_FUNCTION_NAME         "reset"
#define OBJECT_CLEAN_FUNCTION_NAME         "clean_up"

#define INCLUDE_DIRECTORIES                "/include/:/secure/include/"


// simul
#define SIMUL_EFUN_FILE                    ""

#endif // _H_MASTER_DEFS_H_

