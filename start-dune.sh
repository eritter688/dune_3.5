#!/bin/bash
#
# Start DUNE.
#
# Orbital

cd ldbin

./ldmud -DDUNE -D'MUD_NAME="Dune"' --hostname dunemud.net 1> ldmud.log 2>&2 &
#./ldmud -DDUNE -D'MUD_NAME="Dune-2-0"' --hostname dunemud.com >> ldmud.log &
