#!/bin/bash


time g++ -std=c++17 -pedantic -Wall -Wextra table.cpp -o table

[ "$?" ] || exit 1

data=$({
    echo "rights;uid;user;group;size;mon;day;time;name"

    ls test/ -alF \
        | sed -E "s/([^ ]) /\1;/g" \
        | sed -E "s/ //g" \
        | tail -n +2
})

echo "$data"

echo "$data" | ./table
