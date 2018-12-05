openmp: encryption-mp.c
	gcc -o openMP encryption-mp.c -lcrypto -fopenmp
	gcc -o serial encryption-serial.c -lcrypto -fopenmp
	mpicc -o mpi encryption-mpi.c -lcrypto -fopenmp

