#include <cstdio>
#include <omp.h>

int main(int argc, char** argv)
{
  //  omp_set_num_threads(3);
  
  printf("Hallo, OpenMp!\n");
  
#pragma omp parallel
  printf("... hallo nochmal!\n");
  
  printf("Max. Anzahl threads: %d\n", omp_get_max_threads());
}

