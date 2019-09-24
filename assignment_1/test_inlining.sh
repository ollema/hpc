#!/usr/bin/env bash

make clean; make OPT_LEVEL=2 inline_mainfile; ./inline_mainfile;

make clean; make OPT_LEVEL=2 inline_separatefile; ./inline_separatefile;

make clean; make OPT_LEVEL=2 inline_inlined; ./inline_inlined;

make clean;
