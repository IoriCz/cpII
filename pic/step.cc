#include <iostream>
#include <cmath>

#include "./pic.h"

/** Calculate the electric field from Gauss' law, given density n and dx. 
 Note that Ex values are taken at left cell boundaries, while n values are at cell centers (averages).*/
static void calcEx(DArr1& ex, const DArr1& n, double dx)
{
  const int Nx = ex.size();
  ex(0) = 0;
  double sumE = 0.;
  for(int i = 1; i < Nx; ++i){  // Warum wird hier von i=1 an gezaehlt?!?
    ex(i) = dx * (n(i-1) - 1) + ex(i-1);  // Euler Vorwaerts, q/eps_0 = 1, n_mittel = 1
    sumE += ex(i);
  }
  
  for(int i=0; i<Nx; ++i){
    ex(i) -= sumE / Nx;
  }
}

void PIC::moments()
{
  n.fill(0.);
  u.fill(0.);
  T.fill(0.);

  for(int j = 0; j < nParticle; ++j){
    double xj = particles(j).x;
    int iCell = lround( xj/dx + 0.5 ) - 1; // cell index of particle center
    if(iCell < 0){
      iCell += Nx;
      xj += Lx;
    }else if(iCell >= Nx){ 
      iCell -= Nx;
      xj -= Lx;
    }
    
    // weight for neighbor cell: Distance of partcle center from cell center / dx
    double wNeighb = fabs( xj - x(iCell) ) / dx;
    // right or left neighbor cell with periodic conditions
    int iNeighb = ( xj > x(iCell) ) ? (iCell+1) % Nx : (iCell+Nx-1) % Nx;

    // add particle velocity moment in neighbor and own cell
    n(iNeighb) += wNeighb;
    n(iCell) += (1.-wNeighb);
    const double v = particles(j).v;
	u(iNeighb) += v * wNeighb;
	u(iCell) += v * (1.-wNeighb);
  }
  
  for(int j = 0; j < nParticle; ++j){
    double xj = particles(j).x;
    int iCell = lround( xj/dx + 0.5 ) - 1; // cell index of particle center
    if(iCell < 0){
      iCell += Nx;
      xj += Lx;
    }else if(iCell >= Nx){ 
      iCell -= Nx;
      xj -= Lx;
    }
    
    // weight for neighbor cell: Distance of partcle center from cell center / dx
    double wNeighb = fabs( xj - x(iCell) ) / dx;
    // right or left neighbor cell with periodic conditions
    int iNeighb = ( xj > x(iCell) ) ? (iCell+1) % Nx : (iCell+Nx-1) % Nx;

    const double v = particles(j).v;
	T(iNeighb) += powf(v - u(iNeighb), 2) * wNeighb;
	T(iCell) += powf(v - u(iCell), 2) * (1.-wNeighb);
  }

  // compute proper moments
  const double factor = ((double)Nx)/nParticle;
  const double klein = 1e-10;
  for(int i=0; i<Nx; ++i){
    n(i) *= factor;
	u(i) *= factor / (n(i) + klein);
	T(i) *= factor / (n(i) + klein);
  }
  calcEx(Ex, n, dx);
}

/** Leapfrog's 'drift' on particles */
static void drift(Array1<Particle>& parts, double dt)
{
  // push particles
  int Np = parts.size();
  for(int j=0; j<Np; ++j){
    double newX = parts(j).x + dt*parts(j).v;
    // periodic conditions
    if(newX < 0.){
      newX += PIC::Lx;
    }else if(newX > PIC::Lx){
      newX -= PIC::Lx;
    }
    parts(j).x = newX;
  }
}

/** Leapfrog's 'kick' on particles */
static void kick(Array1<Particle>& parts, double dt)
{
  PIC::moments();

  const double dx = PIC::dx;
  const int Nx = PIC::Nx;
  const int Np = parts.size();
  for(int j=0; j<Np; ++j){
    double xj = parts(j).x;
    
    int iBnd = lround( xj/dx ); // index of left cell boundary closest to particle
    double dist = xj - iBnd*dx; // particle's distance to cell boundary
    
    // periodic conditions for index
    iBnd = ( iBnd + Nx ) % Nx; // % is modulo-operator in C
    
    // weight for neighbor boundary: Distance of particle center from cell bndry / dx
    double wNeighb = fabs( dist ) / dx;
    // right or left neighbor bndry with periodicity
    int iNeighb = ( dist > 0. ) ? (iBnd+1) % Nx : (iBnd+Nx-1) % Nx;
    
    // compute average electric field in particle cloud and advance v
    parts(j).v += dt * ( (1-wNeighb)*PIC::Ex( iBnd ) + wNeighb*PIC::Ex( iNeighb ) );
  }
}

void PIC::doInterval(int i)
{
  std::cout << "Starting interval " << i << " up to t = " << i*interval << std::endl;

  double myDt = fmin(dt, interval); // time step for regular leapfrogging
  kick(particles, -0.5*myDt); // sync in: kick v backwards by dt/2
  double toDo = interval; // remaining time to integrate over
  while( toDo >= myDt ){ // do regular kick-drift sequence with myDt
    kick(particles, myDt);
    drift(particles, myDt);
    toDo -= myDt;
  }
  // Now, there's still a remainder time toDo >= 0 for x, 
  // and v still lags behind by myDt/2
  // sync out of Leapfrog: push v by myDt/2. 
  // Plus: Do a toDo/2 kick followed by toDo drift, followed by toDo/2 kick
  kick(particles, 0.5*myDt + 0.5*toDo);
  drift(particles, toDo); // last drift
  kick(particles, 0.5*toDo); // last half-kick
}
