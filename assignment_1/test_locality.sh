#!/usr/bin/env bash

make clean; make OPT_LEVEL=0 locality_naive; ./locality_naive;

make clean; make OPT_LEVEL=0 locality_optimized; ./locality_optimized;

make clean; make OPT_LEVEL=2 locality_naive; ./locality_naive;

make clean; make OPT_LEVEL=2 locality_optimized; ./locality_optimized;

make clean;
