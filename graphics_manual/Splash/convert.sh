#!/bin/bash

set -x

GRIT=/opt/wonderful/thirdparty/blocksds/core/tools/grit/grit

GRAPHICS=../../build/graphics/
OUT=${GRAPHICS}/Splash/

rm -rf ${OUT}
mkdir -p ${OUT}

$GRIT splLayer2.png splLayer3.png -ftc -fh -W2 \
    -gB8 -gb -gT! -gzl -pS \
    -D${OUT} -O${OUT}splPal

$GRIT splLayer1.png -ftc -fh -W2 \
    -gB4 -gt -mR! -m! -gzl -pS \
    -D${OUT} -O${OUT}splSprPal

$GRIT stamp1st.png -ftc -fh -W2 \
    -gB8 -gb -gT! -gzl -m! -p -pn3 \
    -o${OUT}stamp1st
