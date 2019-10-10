###################################################################################################
# settings
###################################################################################################
HPC_USER      = hpcuser004
HPC_HOME      = /home/$(HPC_USER)

# compiler settings
CC            = gcc
CFLAGS        = -Wall -O2
LIBS		  = -lpthread -lm


# objects
OBJS=newton


###################################################################################################
# targets
###################################################################################################
all: clean $(OBJS)

newton: newton.c
	$(CC) -o $@ $(CFLAGS) $(LIBS) $<

submission: newton.c
	@ rm submit.tar.gz
	tar czf submit.tar.gz newton.c Makefile

test: submission
	/home/hpc2019/a2_grading/check_submission.py submit.tar.gz


# helpers
clean:
	@ rm -f $(OBJS)
