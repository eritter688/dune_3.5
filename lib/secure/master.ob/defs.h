/*
 * Master Object Definitions
 *
 */
#ifndef _H_MASTER_DEFS_H_
#define _H_MASTER_DEFS_H_

// additional fortification for master object
#pragma strong_types
#pragma pedantic

// file parsed in preload() & unload()
#define INIT_FILE                          ""
#define SHUT_FILE                          ""

// simul
#define SIMUL_EFUN_FILE                    ""

// messages
#define ENTER_VOID_MESSAGE                 "Everything you see dissolves; luckily, you are transported somewhere else...\n"
#define VOID_FAIL_MESSAGE                  "Catastrophic error - there is no nothing!\n"
#define HEART_BEAT_ERROR_MESSAGE           "Game driver tells you: You have no heart beat !\n" \
                                           "Type 'fix' to restart it.\n"
#define PLAYER_RUNTIME_ERROR_MESSAGE       "Your sensitive mind notices a wrongness in the fabric of space.\n"
#define CONNECT_FAIL_MESSAGE               "Game driver too buggy; please come back later.\n"
#define SHUTTING_DOWN_MESSAGE              "The MUD is closing down now.\n"

#endif // _H_MASTER_DEFS_H_

