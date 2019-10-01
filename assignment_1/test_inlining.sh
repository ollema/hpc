#!/usr/bin/env bash

make clean; make CFLAGS="-Wall -O2" inline_mainfile; ./inline_mainfile;

make clean; make CFLAGS="-Wall -O2" inline_separatefile; ./inline_separatefile;

make clean; make CFLAGS="-Wall -O2" inline_inlined; ./inline_inlined;

make clean;
