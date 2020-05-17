#!/bin/bash
#
# Install DUNE.
#
# Orbital

cd ..
cd ldmud-3.2.17/dune/src

make clean
make install
make install-utils

