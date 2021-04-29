#!/bin/bash

gcc -o output output.c

chmod +x output

./output > filename
cat filename

gcc -o input input.c

chmod +x input

./input 10< filename

./output | ./input
./output 2>&1 | ./input
