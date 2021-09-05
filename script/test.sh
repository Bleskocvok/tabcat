#!/usr/bin/env bash

data=$({
    echo "rights;uid;user;group;size;mon;day;time;name"

    ls test/ -alF \
        | sed -E "s/([^ ]) /\1;/g" \
        | sed -E "s/ //g" \
        | tail -n +2
})


echo "$data" | ./tabcat "$@"

# echo -e "\nDATA"
# echo "$data"
