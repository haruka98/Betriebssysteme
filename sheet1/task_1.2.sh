#!/bin/bash

gcc -o hello hello.c
file hello
file /bin/ls
file ../sheet0/task_0.1.sh
od -t x1 -c -N4 hello
