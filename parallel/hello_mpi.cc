#include <cstdio>
#include <mpi.h>

int main(int argc, char** argv)
{
  int rank = -1;
  int nProc = -1;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  printf("Hallo, ich bin nummer %d von %d\n", rank, nProc);

  MPI_Finalize();

}

