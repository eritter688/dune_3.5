#!/bin/bash
#
# Config script for Dune.
# LDMud 3.6.2
# 
# Orbital

cd ..
cd ldmud-3.6.2/src

./configure \
\
--prefix=/home/dune/dune \
--bindir=/home/dune/dune/ldbin \
--libdir=/home/dune/dune/lib \
--libexecdir=/home/dune/dune/libexec \
\
--with-portno=6789 \
--with-udp-port=6790 \
\
--enable-erq=xerq \
--enable-access-control=yes \
--enable-strict-euids=no \
--enable-filename-spaces=yes \
--enable-use-mysql=yes \
--enable-use-json=yes \
--enable-use-deprecated=no \
--enable-use-python=no \
--enable-malloc-trace=yes \
--enable-malloc-lpc-trace=yes \
--enable-malloc-sbrk-trace=yes \
--enable-eval-cost-trace=yes \
--enable-debug=yes \
--enable-yydebug=yes \
\
--with-optimize=high \
--with-access-file=secure/access/ACCESS.ALLOW \
--with-access-log=secure/access/access.allow.log \
--with-erq-debug=0 \
--with-read-file-max-size=1000000 \
--with-master-name=secure/master \
--with-time-to-clean_up=14400 \
--with-time-to-swap=2629743 \
--with-time-to-swap-variables=2629743 \
--with-time-to-reset=3600 \
--with-evaluator-stack-size=4000 \
--with-max-cost=2000000 \
--with-catch-reserved-cost=8000 \
--with-master-reserved-cost=2048 \
--with-max-array-size=200000 \
--with-max-mapping-size=200000 \
--with-max-callouts=1000 \
--with-max-players=200 \
--with-htable-size=16384 \
--with-itable-size=1024 \
--with-otable-size=4096 \
--with-max-byte-transfer=1250000 \
--with-wizlist-file=data/etc/WIZLIST

