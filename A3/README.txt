CIS3110 Assignment 3

Author: Bejamin Mcconville
###############################################################################
References:
  For dine.c:
	http://man7.org/linux/man-pages/man3/pthread_create.3.html
	was used for passing multiple arguments through a thread

	https://docs.oracle.com/cd/E19205-01/820-0619/gepji/index.html
	was used to understand how to use mutex_lock/unlock with threads

  For holes.c:
    http://www.wou.edu/las/cs/csclasses/cs160/VTCS0/OS/Lessons/MemoryAllocation/index.html
    https://www.quora.com/What-are-the-first-fit-next-fit-and-best-fit-algorithms-for-memory-management
    these two sources were used as a reference of how these algorithms work
	


###############################################################################

How to run:
	-type make while inside the folder
	To execute Dining Philosopher:
	-type ./dine [#philosophers] [# of times each of the philosophers should eat] to execute the program on linux
    To execute Holes in Memory:
	-type ./holes [text file name] to execute the program on linux

##########################################################################################
Files included:
	dine.c		holes.c		makefile    testfile1

##########################################################################################
The Data structure in holes.c uses is a linked list to store the information and an array to act as memory