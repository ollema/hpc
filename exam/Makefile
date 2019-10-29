###################################################################################################
# settings
###################################################################################################
HPC_USER      = hpcuser004
HPC_HOME      = /home/$(HPC_USER)

# compiler settings
CC            = gcc
CFLAGS        = -O0
LIBS		  =


# objects
OBJS=cell_distances cell_distances.o cell_distances_naive cell_distances_naive.o cell_distances_ver1 cell_distances_ver1.o cell_distances_ver2 cell_distances_ver2.o cell_distances_ver3 cell_distances_ver3.o cell_distances_ver4 cell_distances_ver4.o


###################################################################################################
# targets
###################################################################################################
all: clean $(OBJS)

# naive
cell_distances_naive: cell_distances_naive.c
	$(CC) -o $@ $(CFLAGS) -lm $<

# ver1
cell_distances_ver1: cell_distances_ver1.c
	$(CC) -o $@ $(CFLAGS) -march=native $<

# ver2
cell_distances_ver2: cell_distances_ver2.c
	$(CC) -o $@ $(CFLAGS) -march=native $<

# helpers
clean:
	rm -f $(OBJS)
