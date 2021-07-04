#include <cstdio>
#include <mpi.h>

int main(int argc, char** argv)
{
  int rank = -1;
  int nProc = -1;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int rechts = (rank+1) % nProc;
  const int links = (rank-1) % nProc;

  const int len = 10000;

  int nachRechts[len];
  nachRechts[0] = rank*rank;

  int vonLinks[len];

  MPI_Status status;

  if( 0 == (rank&1) ){ // rank gerade?
    MPI_Send(nachRechts, len, MPI_INT, rechts, 0, MPI_COMM_WORLD);
    MPI_Recv(vonLinks, len, MPI_INT, links, 0, MPI_COMM_WORLD, &status);
  }else{
    MPI_Recv(vonLinks, len, MPI_INT, links, 0, MPI_COMM_WORLD, &status);
    MPI_Send(nachRechts, len, MPI_INT, rechts, 0, MPI_COMM_WORLD);
  }

  printf("Ich bin Nummer %d und habe von links die Zahl %d erhalten!\n", 
         rank, vonLinks[0]);

  MPI_Finalize();

}

