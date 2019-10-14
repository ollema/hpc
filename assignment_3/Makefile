###################################################################################################
# settings
###################################################################################################
HPC_USER      = hpcuser004
HPC_HOME      = /home/$(HPC_USER)

# compiler settings
CC            = gcc
CFLAGS        = -Wall -O2 -march=native -pipe
LIBS		  =


# objects
OBJS=cell_distances


###################################################################################################
# targets
###################################################################################################
all: clean $(OBJS)

cell_distances: cell_distances.c
	$(CC) -o $@ $(CFLAGS) $(LIBS) $<

submission: clean cell_distances
	tar czf submit.tar.gz cell_distances.c Makefile

test: submission
	/home/hpc2019/a3_grading/check_submission.py submit.tar.gz


# helpers
clean:
	rm -f $(OBJS) submit.tar.gz
