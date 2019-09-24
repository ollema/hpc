###################################################################################################
# settings
###################################################################################################
HPC_USER      = hpcuser004
HPC_HOME      = /home/$(HPC_USER)

# compiler settings
CC            = gcc
CFLAGS        = -Wall
OPT_LEVEL     = 0 -g

# arb and flint settings
# y or n
USE_ARB_AND_FLINT = n

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
OBJS=time inline_mainfile inline_separatefile inline_inlined locality_naive locality_optimized indirect_naive indirect_optimized writing_to_hdd writing_to_ssd valgrind_demo gdb_demo


###################################################################################################
# targets
###################################################################################################
all: clean $(OBJS)

time: time.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

inline_mainfile: inline_mainfile.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

inline_separatefile: inline_separatefile.c inline_mul_cpx_separatefile.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $< inline_mul_cpx_separatefile.c

inline_inlined: inline_inlined.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

locality_naive: locality_naive.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

locality_optimized: locality_optimized.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

indirect_naive: indirect_naive.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

indirect_optimized: indirect_optimized.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

writing_to_hdd: writing_to_hdd.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

writing_to_ssd: writing_to_ssd.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

valgrind_demo: valgrind_demo.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

gdb_demo: gdb_demo.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<

# helpers
clean:
	@ rm -f $(OBJS)
