#!/usr/bin/env bash

gcc -O0 -S -o time_0.s time.c

gcc -O3 -S -o time_3.s time.c
