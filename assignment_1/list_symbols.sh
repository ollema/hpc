#!/usr/bin/env bash

echo -e "symbols for inline_mainfile:\n"
gcc -O2 -o inline_mainfile.o inline_mainfile.c
nm inline_mainfile.o
rm -f inline_mainfile.o

echo -e "\n\nsymbols for inline_separatefile:\n"
gcc -O2 -o inline_separatefile.o inline_separatefile.c inline_mul_cpx_separatefile.c
nm inline_separatefile.o
rm -f inline_separatefile.o
