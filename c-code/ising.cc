#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "array.h"

// Gittergroesse: Spins pro Raumrichtung
const int M = 100;

// Das Array der Spins als Integer-Zahlen 
IntArray2 spins;

// Energie und Magenetisierung, ebenfalls als (long)
long int e;
long int m;

const double beta = 1 / 2.3; // 1 / kT
const long int nMetro = 100000; // Anzahl Metropolis-Iterationen pro Durchlauf
const int nRound = 100; // Anzahl Durchlaeufe
const long int nTherm = nRound / 10; // Anzahl Thermalisierungs-Runden

long int eSum = 0;
long int mSum = 0;

// Initialisierung zum Programmstart
static void init()
{
  // Allokation der M*M Spinwerte
  spins.allocate(M, M);
  spins.fill(1);

  // Energie und Magnetisierung
  e = -2*M*M;
  m = M*M;
}

// Aufraeumen zum Programmende
static void finish()
{
  // De-Allokation der Spins
  spins.free();
}

// Ein Standard-Zufallsgenerator ist drand48()
static double zufall() { return drand48(); }

static void metro()
{
  for(int k = 0; k < nMetro; ++k){

    //Finde zufaelligen Spin ...
    int zeile = (int) ( M*zufall() );
    int spalte = (int) ( M*zufall() );
    
    // schaue dessen Nachbarspins an (down, up, left, right)
    int sd = spins( zeile-1, (spalte + M - 1) % M );
    int su = spins( zeile, (spalte + 1) % M );
    int sl = spins( (zeile + M - 1) % M , spalte );
    int sr = spins( (zeile + 1) % M , spalte );
    
    // Energieaenderung, wenn dieser Spin umgedreht wird:
    // Faktor 2: Spinaenderung um +- 2 ( +1 <-> -1) in jedem der 4 Paare
    int delta_e = 2 * spins(zeile, spalte) * (sd+su+sl+sr);
    
    // Relativer Boltzmann-Faktor bei Spinaenderung
    double r = exp( -beta*delta_e );
      
    // Metropolis-Entscheidung
    if ( r >= 1. || r > zufall() ){
      // Neue Energie geringer, oder Zufallszahl aus [0 1) kleiner als r
      // => Spin umdrehen und Gesamtenergie anpassen
      spins(zeile, spalte) = -spins(zeile, spalte);
      e += delta_e;
      m += 2*spins(zeile, spalte);
    }
    
    // Statistik
    eSum += e;
    mSum += m;
  }
}

int main(int argc, char** argv)
{
  init();
  
  // Thermalisierung
  for(int r = 0; r < nTherm; ++r){
    metro();
  }

  // setze Statistik zurueck
  eSum = 0;
  mSum = 0;

  // Sample
  printf("# Runde   <e>   <m>\n");

  for(int r = 1; r <= nRound; ++r){
    metro();

    // Mittelwerte berechnen (pro Spin)
    double w = 1. / ( r * M*M * nMetro );
    printf(" %5d %10.7g %10.7g\n", r, w*eSum, w*mSum);
  }
  
  finish();
}
    

