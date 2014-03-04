seq:
	gcc -Wall -O -o seq totientrange.c

mpi: 
	mpicc -Wall -O -o mpi mpitotientrange.c

gph:
	/u1/staff/pm175/sw/ghc/7.4.1/x86_64/bin/ghc-7.4.1 -threaded -rtsopts -eventlog --make -package-conf=/u1/staff/pm175/sw/ghc/_dot_ghc/x86_64-linux-7.4.1/package.conf.d gphtotientrange.hs -o gph

haskell:
	ghc -O2 -o haskell htotientrange.hs

profhs:
	ghc -prof -auto-all -rtsopts -O2 -o phaskell htotientrange.hs

omp:
	gcc -fopenmp omptotientrange.c -o omp

clean:
	rm *.o  *.hi gph mpi haskell seq omp
