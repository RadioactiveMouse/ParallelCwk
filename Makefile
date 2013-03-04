seq:
	gcc -Wall -O -o seq totientrange.c

mpi: 
	mpdboot --file=m4 --totalnum=4 --verbose
	mpicc -Wall -O -o mpi mpitotientrange.c
