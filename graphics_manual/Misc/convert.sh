#!/bin/bash

set -x

BLOCKSDS="${BLOCKSDS:-/opt/blocksds/core/}"
GRIT=$BLOCKSDS/tools/grit/grit

GRAPHICS=../../build/graphics/
OUT=${GRAPHICS}/Misc/

rm -rf ${OUT}
mkdir -p ${OUT}

# This should be restricted to 16 colors
$GRIT aButton.png battleGauge.png textCursor.png -ftc -fh -W2 \
    -gB4 -gt -gTFF00FF -m! -mR! -pS \
    -D${OUT} -O${OUT}spr16Pal

# Reserve the last 16 colors of the 256 color palette for other sprites
$GRIT playerEmblem.png -ftc -fh -W2 \
    -gB8 -gt -gTFF00FF -gzl -m! -mR! -pn240 \
    -o${OUT}playerEmblem

$GRIT textBox.png -ftc -fh -W2 \
    -gB4 -gt -gTFF00FF -gzl -m! -mR! -pn16 \
    -o${OUT}textBox

$GRIT textFont.png -ftc -fh -W2 \
    -gB4 -gt -gT0000FF -gzl -m! -mR! -pn4 \
    -o${OUT}textFont
