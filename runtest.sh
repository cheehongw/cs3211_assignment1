#!/bin/bash

for f in $1/*
do
    echo $f
    if (./grader engine < $f 2>&1 | grep -a "passed" > /dev/null); then
        echo "passed"
    else
        echo "failed"
    fi
done