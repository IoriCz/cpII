#include <cstdlib>
#include <cmath>

#include "./pic.h"

static double zuf() { return drand48(); }

void PIC::init()
{
  // set parameters:
  nParticle = 1000000;

  // iteration:
  interval = 0.5;
  nInterval = 10;
  dt = 0.05;

  Lx = 3.;
  Nx = 1000;
  dx = Lx / Nx;

  outFileNameStem = "data/";
  
  double vtherm = 1.;

  // initialize particles
  particles.allocate(nParticle);
  for(int ip=0; ip<nParticle; ++ip){
    Particle& p = particles(ip);
    p.x = Lx * ip / nParticle;
	p.x += 0.001*sin(2.*M_PI* p.x / Lx);
    //p.v = 0.02*sin(2.*M_PI* p.x / Lx);
	p.v = (ip%2)==0 ? 0.3 : -0.3;
  }

  // allocate grid and fields
  x.allocate(Nx);
  n.allocate(Nx);
  u.allocate(Nx);
  T.allocate(Nx);
  Ex.allocate(Nx);

  // set x-positions of cell centers
  for(int i = 0; i < Nx; ++i){
    x(i) = (i + 0.5)*dx;
  }
}

void PIC::finish()
{
  // clear grid and fields
  x.free();
  n.free();
  u.free();
  T.free();
  Ex.free();

  particles.free();
}

