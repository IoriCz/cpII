#ifndef PIC_PIC_H
#define PIC_PIC_H

#include "./array.h"

struct Particle
{
  double x; // position
  double v; // velocity
};

// Abbreviate
typedef Array1<double> DArr1;

/** In this kind of implementation, PIC is basically a namespace for (almost-)global
    variables. */
struct PIC
{
  /** ++++ Simulation parameters */

  /** Number of computational particles. */
  static int nParticle;

  /** Integration occurs over several time intervals. This is the time span of each interval. */
  static double interval;

  /** This is the number of intervals. */
  static int nInterval; // # of time intervals in total

  /** The (maximum) time step for leapfrogging. */
  static double dt; // maximum time step to use

  /** System length */
  static double Lx;

  /* Number of grid cells in x-direction and cell width. */
  static int Nx;
  static double dx; // derived from above

  /** ++++ Arrays to hold grid quntities: */

  /** x-positions of cell centers. */
  static DArr1 x;

  /** Accumulated  density in cells. */
  static DArr1 n; //

  /** Averaged velocity in cells. */
  static DArr1 u;

  /** Averaged temperature in cells. */
  static DArr1 T;

  /** Electric field at left boundary of each cell. */
  static DArr1 Ex;

 
  /** ++++ Array of particles. */
  static Array1<Particle> particles;

  /** ++++ Functions */
  
  /* Initialize parameters and data. */
  static void init();

  /** Start and run simulation. */
  static void run();

  /** Clear allocated data. */
  static void finish();

  /** Compute particle moments on grid. */
  static void moments();

  /* Integrate for one time interval. */
  static void doInterval(int seq);

  /* Write particles to file. Each call will create a new particle and field data file.
     File names are based on outFileNameStem, files are numbered sequentially. */
  static void output();
  static const char* outFileNameStem;
};

#endif
