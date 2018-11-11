#!/bin/zsh

if [ -e "./dumbot-v0" ]; then
    ./dumbot-v0 $1
else
    echo "First run compile.sh to compile the code"
fi
