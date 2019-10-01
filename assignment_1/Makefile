###################################################################################################
# settings
###################################################################################################
HPC_USER      = hpcuser004
HPC_HOME      = /home/$(HPC_USER)

# compiler settings
CC            = gcc
CFLAGS        = -Wall -O2

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
.PHONY=time inline_mainfile inline_separatefile inline_inlined locality_naive locality_optimized indirect_naive indirect_optimized writing_to_hdd writing_to_ssd valgrind_demo gdb_demo
OBJS=time inline_mainfile inline_separatefile inline_inlined locality_naive locality_optimized indirect_naive indirect_optimized writing_to_hdd writing_to_ssd valgrind_demo gdb_demo


###################################################################################################
# targets
###################################################################################################
all: clean $(OBJS)

%: %.o
	$(CC) -o $@ $(CFLAGS) $(LIBS) $<

inline_separatefile: inline_separatefile.o inline_mul_cpx_separatefile.o
	$(CC) -o $@ $(CFLAGS) $(LIBS) inline_separatefile.o inline_mul_cpx_separatefile.o

# helpers
clean:
	@ rm -f $(OBJS) *.o
