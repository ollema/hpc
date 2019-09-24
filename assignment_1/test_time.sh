#!/usr/bin/env bash

make clean; make OPT_LEVEL=0 time; ./time;

make clean; make OPT_LEVEL=g time; ./time;

make clean; make OPT_LEVEL=1 time; ./time;

make clean; make OPT_LEVEL=2 time; ./time;

make clean; make OPT_LEVEL=3 time; ./time;

make clean; make OPT_LEVEL=s time; ./time;

make clean;
