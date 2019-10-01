#!/usr/bin/env bash

make clean; make CFLAGS="-Wall -O0" indirect_naive; ./indirect_naive;

make clean; make CFLAGS="-Wall -O0" indirect_optimized; ./indirect_optimized;

make clean; make CFLAGS="-Wall -O2" indirect_naive; ./indirect_naive;

make clean; make CFLAGS="-Wall -O2" indirect_optimized; ./indirect_optimized;

make clean;
