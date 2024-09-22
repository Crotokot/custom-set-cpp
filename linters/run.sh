#!/usr/bin/env bash

set -o pipefail

function print_header() {
    echo -e "\n***** ${1} *****"
}

function check_log() {
    LOG=$( { ${1}; } 2>&1 )
    STATUS=$?
    echo "$LOG"
    if echo "$LOG" | grep -q -E "${2}"
    then
        exit 1
    fi

    if [ $STATUS -ne 0 ]
    then
        exit $STATUS
    fi
}

print_header "RUN cppcheck"
check_log "cppcheck include/*.hpp tests/unittests.cpp tests/tests.hpp main.cpp --std=c++17 -I include -I tests --enable=all --inconclusive --check-config --error-exitcode=1 --suppressions-list=linters/suppress.txt" "\(information\)"

print_header "RUN cpplint"
check_log "cpplint --extensions=cpp,hpp include/*.hpp tests/*.hpp tests/unittests.cpp main.cpp" "Can't open for reading"

print_header "SUCCESS"