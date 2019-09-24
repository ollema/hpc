#!/usr/bin/env bash

make clean; make OPT_LEVEL=0 indirect_naive; ./indirect_naive;

make clean; make OPT_LEVEL=0 indirect_optimized; ./indirect_optimized;

make clean; make OPT_LEVEL=2 indirect_naive; ./indirect_naive;

make clean; make OPT_LEVEL=2 indirect_optimized; ./indirect_optimized;

make clean;
