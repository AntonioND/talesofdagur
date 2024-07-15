#!/bin/bash

set -x

GRIT=/opt/blocksds/core/tools/grit/grit

GRAPHICS=../../arm9/graphics/
OUT=${GRAPHICS}/Menu/

rm -rf ${OUT}
mkdir -p ${OUT}

$GRIT gameOver.bmp -ftc -fh -W2 \
    -gB4 -gt -gzl -m -mzl -p -pn3 \
    -o${OUT}gameOver

$GRIT mainMenuBack.bmp mainMenuFront.bmp -ftc -fh -W2 \
    -gB4 -gt -m -gzl -mzl -pS \
    -D${OUT} -O${OUT}mainMenuPal
