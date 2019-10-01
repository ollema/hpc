#!/usr/bin/env bash

make clean; make CFLAGS="-Wall -O0" time; ./time;

make clean; make CFLAGS="-Wall -Og" time; ./time;

make clean; make CFLAGS="-Wall -O1" time; ./time;

make clean; make CFLAGS="-Wall -O2" time; ./time;

make clean; make CFLAGS="-Wall -O3" time; ./time;

make clean; make CFLAGS="-Wall -Os" time; ./time;

make clean;
