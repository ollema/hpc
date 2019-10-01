#!/usr/bin/env bash

make clean; make CFLAGS="-Wall -O0" locality_naive; ./locality_naive;

make clean; make CFLAGS="-Wall -O0" locality_optimized; ./locality_optimized;

make clean; make CFLAGS="-Wall -O2" locality_naive; ./locality_naive;

make clean; make CFLAGS="-Wall -O2" locality_optimized; ./locality_optimized;

make clean;
