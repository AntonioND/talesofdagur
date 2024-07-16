#!/bin/bash

set -x

GRIT=/opt/blocksds/core/tools/grit/grit

GRAPHICS=../../build/graphics/
OUT=${GRAPHICS}/Menu/

rm -rf ${OUT}
mkdir -p ${OUT}

$GRIT gameOver.png -ftc -fh -W2 \
    -gB4 -gt -gzl -m -mzl -p -pn3 \
    -o${OUT}gameOver

$GRIT mainMenuBack.png mainMenuFront.png -ftc -fh -W2 \
    -gB4 -gt -m -gzl -mzl -pS \
    -D${OUT} -O${OUT}mainMenuPal

$GRIT palGrad.png -ftc -fh -W2 \
    -gB16 -gb \
    -o${OUT}palGrad
