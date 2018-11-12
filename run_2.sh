#!/bin/bash

if [ -e "./a.out" ]; then
    ./a.out $1
else
    echo "First run compile.sh to compile the code"
fi
