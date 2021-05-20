#include <iostream>
#include <cmath>

#include "./pic.h"

// Definition of static (class-) variables
int PIC::nParticle;
double PIC::interval;
int PIC::nInterval;
double PIC::dt;

double PIC::Lx;
int PIC::Nx;
double PIC::dx;

DArr1 PIC::x;
DArr1 PIC::n;
DArr1 PIC::u;
DArr1 PIC::T;
DArr1 PIC::Ex;

Array1<Particle> PIC::particles;

const char* PIC::outFileNameStem;

/* Main entry function. */
int main(int argc, char** argv)
{
  std::cout << "Hello, particles!" << std::endl;

  PIC::init();
  PIC::output(); // initial output
  PIC::run();
  PIC::finish();

  return 0;
}

void PIC::run()
{
  std::cout << "Time interval = " << interval
            << ", #intervals = " << nInterval
            << ", dt = " << dt
            << ", substeps: " << ceil(interval/dt) << std::endl;

  // iteration loop
  for(int iter = 1; iter <= nInterval; ++iter){
    doInterval(iter);
    output();
  }
}

