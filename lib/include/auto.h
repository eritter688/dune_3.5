/*
 * Dune auto-include file.
 * All defines/pragmas listed here will be applied when compiling an object.
 *
 *
 */
#ifndef _H_AUTO_INCLUDE_H_
#define _H_AUTO_INCLUDE_H_

// Should use '-DDune' on mud start.
#ifndef DUNE
#define DUNE
#endif

// Should use -D'MUD_NAME'="Dune" on mud start.
#ifndef MUD_NAME
#if query_mud_port() == 6789
#define MUD_NAME            "Dune"
#elif query_mud_port() == 4242
#define MUD_NAME            "DevDune"
#else
#define MUD_NAME            "Dune?"
#endif
#endif

#endif // _H_AUTO_INCLUDE_H_
