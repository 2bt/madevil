#!/bin/sh

d=$(pwd)
cd ~/stuff/emsdk
. ./emsdk_env.sh
cd $d

emcc \
    src/*cpp \
    --embed-file assets \
    -s USE_SDL=2 -s USE_SDL_IMAGE=2 \
    -s SDL2_IMAGE_FORMATS='["png"]' \
    -Os \
    -o index.js
