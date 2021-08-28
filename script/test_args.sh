#!/usr/bin/env bash


time g++ src/main.cpp -o table -std=c++17 -pedantic -Wall -Wextra

[ "$?" ] || exit 1

B='\033[1m'
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

function pass()
{
    ./table $@ || { echo -e "${RED}nok; expected PASS, but failed\t./table $@${NC}";
                    exit 1; }
    echo -e "${B}${GREEN}ok${NC}"
}

function fail()
{
    ./table $@ && { echo -e "${RED}nok; expected FAIL, but passed\t./table $@${NC}";
                    exit 1; }
    echo -e "${B}${GREEN}ok${NC}"
}


pass --help
fail --helpa
pass -TBLR
fail --TBLR
