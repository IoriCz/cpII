#include <cstdio>
#include <omp.h>

int main(int argc, char** argv)
{
  const int nThr = 5;
  const long N = 10000000;
  
  /*
  long int summe = 0l;
  omp_set_num_threads(nThr);

  #pragma omp parallel
  for(int i = 0; i < N; i++){

    #pragma omp critical
    summe += 1;
  }

  printf("%d mal %ld ist %ld\n", nThr, N, summe);
}*/

  ///*

  long int summe = 0l;
  omp_set_num_threads(nThr);
  
#pragma omp parallel
  {
    long int meineSumme = 0l;
    for(int i = 0; i < N; i++){
      meineSumme += 1;
    }

#pragma omp critical
    summe += meineSumme;
  }

  printf("%d mal %ld ist %ld\n", nThr, N, summe);
}

//*/
