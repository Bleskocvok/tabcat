#!/usr/bin/env bash


g++ src/tabcat.cpp -o table -std=c++17 -pedantic -Wall -Wextra || exit 1

B='\033[1m'
RED='\033[0;31m'
GREEN='\033[0;32m'
LGREY='\033[1;30m'
NC='\033[0m'
PASS_NOK='nok; expected PASS, but failed:\t\t'
FAIL_NOK='nok; expected FAIL, but passed:\t\t'

function ind() { sed "s/^/\t/g"; }

function header()
{
    echo
    echo -e "${NC}./table $@${LGREY}"
    echo "" | ./table $@ 2>&1 | ind | head -n 3
}

function pass()
{
    header $@
    echo "" | ./table $@ &> /dev/null || {
        echo -e "${RED}${PASS_NOK}./table $@${NC}" | ind;
        exit 1;
    }
    echo -e "${B}${GREEN}ok${NC}" | ind
}

function fail()
{
    header $@
    echo "" | ./table $@ &> /dev/null && {
        echo -e "${RED}${FAIL_NOK}./table $@${NC}" | ind;
        exit 1;
    }
    echo -e "${B}${GREEN}ok${NC}" | ind
}

pass --help
fail --helpa
fail --
fail -
fail --help=ajo
fail --help=
fail --TBLR
fail --delimiter
pass --delimiter a
pass --format=md
fail --format=blablalbalblalbal
pass -TBLR
fail -TBLRb
fail -bTBLR
fail -TBbLR
pass -d -
pass --delimiter -
