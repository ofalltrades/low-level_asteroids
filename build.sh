#!/bin/bash

clang++ src/main.cpp -o build/Release/low-level_asteroids \
        -I/Library/Frameworks/SDL2.framework/Headers \
        -F/Library/Frameworks -framework SDL2 \
        --std=c++14

clang++ src/main.cpp -o build/Debug/low-level_asteroids \
        -g -I/Library/Frameworks/SDL2.framework/Headers  \
        -F/Library/Frameworks -framework SDL2 \
        --std=c++14

case "$1" in
  -x) build/Release/low-level_asteroids;;
esac
