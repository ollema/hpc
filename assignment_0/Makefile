.PHONY: all clean

###################################################################################################
# settings
###################################################################################################
HPC_USER      = hpcuser004
HPC_HOME      = /home/$(HPC_USER)

# compiler settings
CC            = gcc
CFLAGS        = -Wall
OPT_LEVEL     = 2

# arb and flint settings
# y or n
USE_ARB_AND_FLINT = y

LIB_PATHS         = -L$(HPC_HOME)/local_arb/lib -L$(HPC_HOME)/local_flint/lib
INCLUDE_PATHS     = -I$(HPC_HOME)/local_arb/include -I$(HPC_HOME)/local_flint/include
LINKER_OPTS       = -Wl,-rpath=$(HPC_HOME)/local_arb/lib,-rpath=$(HPC_HOME)/local_flint/lib
LIBRARIES         = -larb -lflint

ifeq ($(USE_ARB_AND_FLINT), y)
  LIBS=$(LIB_PATHS) $(INCLUDE_PATHS) $(LINKER_OPTS) $(LIBRARIES)
else
  LIBS=
endif

# objects
OBJS=poly_roots stack_alloc heap_alloc memory_fragmentation writing_to_file printargs


###################################################################################################
# targets
###################################################################################################
all: clean $(OBJS)

poly_roots: poly_roots.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

stack_alloc: stack_alloc.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

heap_alloc: heap_alloc.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

memory_fragmentation: memory_fragmentation.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

writing_to_file: writing_to_file.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

printargs: printargs.c
	@$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

# helpers
clean:
	rm -f $(OBJS)
