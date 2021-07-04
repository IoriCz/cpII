GPP_OMP := g++ -fopenmp
GPP_MPI := mpic++

all: hello_omp summe hello_mpi nachbar pi

hello_omp: hello_omp.cc
	$(GPP_OMP) -o $@ $<

summe: summe.cc
	$(GPP_OMP) -o $@ $<

hello_mpi: hello_mpi.cc
	$(GPP_MPI) -o $@ $<

nachbar: nachbar.cc
	$(GPP_MPI) -o $@ $<

pi: pi.cc
	$(GPP_MPI) -o $@ $<

clean:
	rm -f hello_omp summe hello_mpi nachbar pi
