#!/bin/bash

set -x

GRIT=/opt/blocksds/core/tools/grit/grit

GRAPHICS=../../arm9/graphics/
OUT=${GRAPHICS}/Misc/

rm -rf ${OUT}
mkdir -p ${OUT}

$GRIT battleGauge.bmp textCursor.bmp -ftc -fh -W2 \
    -gB4 -gt -gTFF00FF -m! -mR! -pS \
    -D${OUT} -O${OUT}spr16Pal

# Reserve the last 16 colors of the 256 color palette for other sprites
$GRIT playerEmblem.bmp -ftc -fh -W2 \
    -gB8 -gt -gTFF00FF -gzl -m! -mR! -pn240 \
    -o${OUT}playerEmblem

$GRIT textBox.bmp -ftc -fh -W2 \
    -gB4 -gt -gTFF00FF -gzl -m! -mR! -pn16 \
    -o${OUT}textBox

$GRIT textFont.bmp -ftc -fh -W2 \
    -gB4 -gt -gT0000FF -gzl -m! -mR! -pn4 \
    -o${OUT}textFont
