#!/usr/bin/env bash

B='\033[1m'
RED='\033[0;31m'
GREEN='\033[0;32m'
LGREY='\033[1;90m'
NC='\033[0m'
PASS_NOK='nok; expected PASS, but failed:\t\t'
FAIL_NOK='nok; expected FAIL, but passed:\t\t'

export APP="./tabcat"

function ind() { sed "s/^/\t/g"; }

function header()
{
    echo
    echo -e "${NC}${APP} $@${LGREY}"
    echo "" | $APP $@ 2>&1 | ind | head -n 3
}

function perform()
{
    if echo "" | $APP $@ &> /dev/null;
    then
        return 0
    else
        return 1
    fi
}

function pass()
{
    header $@
    perform $@ || {
        echo -e "${RED}${PASS_NOK}${APP} $@ ${NC}" | ind;
        exit 1;
    }
    echo -e "${B}${GREEN}ok${NC}" | ind
}

function fail()
{
    header $@
    perform $@ && {
        echo -e "${RED}${FAIL_NOK}${APP} $@ ${NC}" | ind;
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
