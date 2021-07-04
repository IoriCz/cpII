#include <cstdio>
#include <mpi.h>
#include <math.h>

int main(int argc, char** argv)
{
  int rank = -1; 
  int nProc = -1;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double result = 0.;
  const long N = 10000000;
  srand(12345 * rank);

  for(long i = 0; i < N; i++){
    double x = ( (double)rand() ) / RAND_MAX;
    double y = ( (double)rand() ) / RAND_MAX;
    if(x*x + y*y < 1.){
      result += 1.;
    }
  }
  
  MPI_Allreduce(MPI_IN_PLACE, &result, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  result *= 4.;
  if(0 == rank){
    result /= (N*nProc);
    printf("Ergebnis aus %ld samples: %lf Fehler: %le\n", 
           nProc*N, result, result-2.*acos(0.));
  }
  MPI_Finalize();
}

