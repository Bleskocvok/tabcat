#!/usr/bin/env bash

time clang++ src/tabcat.cpp -o tabcat \
    -std=c++17  \
    -pedantic   \
    -Wall       \
    -Wextra     \
    || exit 1
