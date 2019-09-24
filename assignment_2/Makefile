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
OBJS=example


###################################################################################################
# targets
###################################################################################################
all: clean $(OBJS)

example: example.c
	$(CC) -o $@ $(CFLAGS) -O$(OPT_LEVEL) $(LIBS) $<


# helpers
clean:
	@ rm -f $(OBJS)
