#!/usr/bin/env bash

time g++ src/tabcat.cpp -o tabcat \
    -std=c++17  \
    -pedantic   \
    -Wall       \
    -Wextra     \
    -static -static-libgcc -static-libstdc++ \
    || exit 1
