#!/bin/bash

echo ${PATH}

which wc
which lsof

gcc -std=c99 -Wall -o starter starter.c
chmod +x starter
chmod +x bob
export PATH=${PATH}:.
./starter
./starter "lovely fellow"
