#!/bin/bash

set -x

GRIT=/opt/blocksds/core/tools/grit/grit

GRAPHICS=../../arm9/graphics/
OUT=${GRAPHICS}/Levels/

rm -rf ${OUT}
mkdir -p ${OUT}

do_map() {
    NUM=$1
    EXT=$2

    $GRIT level${NUM}Up.$EXT level${NUM}Down.$EXT level${NUM}Ground.$EXT -ftc -fh -W2 \
        -gB8 -gt -gTFF00FF -gzl -pS -m -mR8 \
        -D${OUT} -O${OUT}level${NUM}Pal

    $GRIT level${NUM}Col.$EXT -ftc -fh -W2 \
        -gB8 -gt -gT000000 -g! -m -mLa -p! \
        -o${OUT}level${NUM}Col
}

do_map 1 bmp
do_map 2 bmp
do_map 3 bmp
do_map 4 bmp
do_map 5 bmp
do_map 6 png
do_map 7 bmp
do_map 8 bmp
do_map 9 bmp
do_map 10 bmp
do_map 11 bmp
do_map 12 bmp
do_map 13 png
do_map 14 bmp
do_map 15 bmp
do_map 16 png
do_map 17 bmp
do_map 18 bmp
