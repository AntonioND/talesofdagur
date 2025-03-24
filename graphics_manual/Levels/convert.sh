#!/bin/bash

set -x

BLOCKSDS="${BLOCKSDS:-/opt/blocksds/core/}"
GRIT=$BLOCKSDS/tools/grit/grit

GRAPHICS=../../build/graphics/
OUT=${GRAPHICS}/Levels/

rm -rf ${OUT}
mkdir -p ${OUT}

do_map() {
    NUM=$1

    $GRIT level${NUM}Up.png level${NUM}Down.png level${NUM}Ground.png -ftc -fh -W2 \
        -gB8 -gt -gTFF00FF -gzl -pS -m -mR8 \
        -D${OUT} -O${OUT}level${NUM}Pal

    $GRIT level${NUM}Col.png -ftc -fh -W2 \
        -gB8 -gt -gT000000 -g! -m -mLa -p! \
        -o${OUT}level${NUM}Col
}

do_map 1
do_map 2
do_map 3
do_map 4
do_map 5
do_map 6
do_map 7
do_map 8
do_map 9
do_map 10
do_map 11
do_map 12
do_map 13
do_map 14
do_map 15
do_map 16
do_map 17
do_map 18
