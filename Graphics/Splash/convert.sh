#!/bin/bash

set -x

GRIT=/opt/blocksds/core/tools/grit/grit

GRAPHICS=../../arm9/graphics/
OUT=${GRAPHICS}/Splash/

rm -rf ${OUT}
mkdir -p ${OUT}

$GRIT splLayer2.bmp splLayer3.bmp -ftc -fh -W3 \
    -gB8 -gb -gT! -gzl -pS \
    -D${OUT} -O${OUT}splPal

$GRIT splLayer1.bmp -ftc -fh -W3 \
    -gB4 -gt -mR! -m! -gzl -pS \
    -D${OUT} -O${OUT}splSprPal
