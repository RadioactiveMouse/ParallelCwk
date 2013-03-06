seq:
	gcc -Wall -O -o seq totientrange.c

mpi: 
	mpdboot --file=m4 --totalnum=4 --verbose
	mpicc -Wall -O -o mpi mpitotientrange.c

gph:
	/u1/staff/pm175/sw/ghc/7.4.1/x86_64/bin/ghc-7.4.1 -threaded -rtsopts -eventlog --make -package-conf=/u1/staff/pm175/sw/ghc/_dot_ghc/x86_64-linux-7.4.1/package.conf.d gphtotientrange.hs -o gph

haskell:
	ghc -O2 -o haskell htotientrange.hs

openmp:
	gcc -fopenmp omptotientrange.c -o openmp

clean:
	rm *.o  *.hi gph mpi haskell seq openmp
