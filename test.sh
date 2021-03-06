#!/usr/bin/env bash

cat read.c main.c > /tmp/a
./cat read.c main.c > /tmp/b

diff /tmp/a /tmp/b && printf "\nTEST SUCCESS\n\n" || printf "\nTEST FAILURE\n\n"

rm -f /tmp/{a,b}
