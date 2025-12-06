#!/bin/bash

INIT_PATH="$PWD"

if [ ! -d 3rdParty/SDL/build ];
then
        cd 3rdParty/SDL
        mkdir build
        cd build
        cmake ..
        make -j
fi
cd "$INIT_PATH"

if [ ! -d 3rdParty/SDL_image/build ];
then
        cd 3rdParty/SDL_image
        mkdir build
        cd build
        SDL3_DIR="$INIT_PATH/3rdParty/SDL/build" cmake ..
        make -j
fi
cd "$INIT_PATH"

if [ ! -d 3rdParty/SDL_ttf/build ];
then
        cd 3rdParty/SDL_ttf
        mkdir build
        cd build
        SDL3_DIR="$INIT_PATH/3rdParty/SDL/build" cmake ..
        make -j
fi
cd "$INIT_PATH"

export LD_LIBRARY_PATH=3rdParty/SDL/build:3rdParty/SDL_image/build:3rdParty/SDL_ttf/build:$LD_LIBRARY_PATH

make -j
