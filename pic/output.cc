#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include "./pic.h"

static FILE* openFile(const char* filename)
{
  FILE* fp = fopen(filename, "wb");
  if( nullptr == fp ){
    std::cout << "Can't open output file '" << filename << "'\n";
    exit(1);
  }
  return fp;
}

static int outSeq = 0;

void PIC::output()
{
  // write particle data: Create output file name from seq
  std::ostringstream oss;
  oss << outFileNameStem << "partData." << std::setw(4) << std::setfill('0') << outSeq;

  
 // restrict #particles to write out
  const int maxOut = 1000;
  const int nOut = nParticle > maxOut ? maxOut : nParticle;

  FILE* fp = openFile( oss.str().c_str() );
  fprintf(fp, "# np = %d time = %g\n*", nOut, outSeq*interval);
  for(int ip=0; ip<nOut; ++ip){
    int rp = nParticle * drand48();
    fwrite( & particles(rp).x, sizeof(double), 1, fp);
    fwrite( & particles(rp).v, sizeof(double), 1, fp);
  }
  fclose(fp);

  // write field data, compute moments first
  moments();
  
  std::ostringstream foss;
  foss << outFileNameStem << "fieldData." << std::setw(4) << std::setfill('0') << outSeq;

  fp = openFile( foss.str().c_str() );
  fprintf(fp, "# nx = %d time = %g\n*", Nx, outSeq*interval);

  // write x, n, u, T, Ex to file
  fwrite( x.data(), sizeof(double), Nx, fp);
  fwrite( n.data(), sizeof(double), Nx, fp);
  fwrite( u.data(), sizeof(double), Nx, fp);
  fwrite( T.data(), sizeof(double), Nx, fp);
  fwrite( Ex.data(), sizeof(double), Nx, fp);

  fclose(fp);

  outSeq++;
}
