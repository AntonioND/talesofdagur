#!/bin/bash

set -x

pushd Levels
bash convert.sh
popd

pushd Menu
bash convert.sh
popd

pushd Misc
bash convert.sh
popd

pushd Splash
bash convert.sh
popd
