#include <cstdio>
#include <cstdlib>
#include <cmath>

// Gittergroesse: Spins pro Raumrichtung
const int M = 100;

// Das Array der Spins als Integer-Zahlen 
int* s = nullptr;

// Energie und Magenetisierung, ebenfalls als (long)
long int e;
long int m;

const double beta = 1 / 2.5; // 1 / kT
const long int nSample = 100000; // Anzahl Metropolis-Iterationen pro Durchlauf
const int nRound = 100; // Anzahl Durchlaeufe
const long int nTherm = nRound / 10; // Anzahl Thermalisierungs-Runden

long int eSum = 0;
long int mSum = 0;

// Initialisierung zum Programmstart
static void init()
{
	// Allokation der M*M Spinwerte
	s =  new int[M*M];
	// Setze alle Spins 'up'
	for(int i=0; i < M*M; ++i){
		s[i] = 1;
	}
	e = -2*M*M;
	m = M*M;
}

// Aufraeumen zum Programmende
static void finish()
{
	// De-Allokation der Spins
	delete[] s;
	s = nullptr;
}

// Referenz auf einen der Spins
// Diese Funktion bildet Zeilen- und  Spaltenindex der Matrix auf einen
// eindimensionalen Index im allokierten Speicher ab
// Die Indizes laufen hier von 0 .. (M-1)
static int& spin(int zei, int spa) {
	return s[ zei + M*spa ];
}

// Ein Standard-Zufallsgenerator ist drand48()
static double zufall() { return drand48(); }

static void metro()
{
	for(int k = 0; k < nSample; ++k){
		
		//Finde zufaelligen Spin ...
		int zeile = (int) ( M*zufall() );
		int spalte = (int) ( M*zufall() );
		
		// schaue dessen Nachbarspins an (down, up, left, right)
		int sd = spin( zeile, (spalte + M - 1) % M );
		int su = spin( zeile, (spalte + 1) % M );
		int sl = spin( (zeile + M - 1) % M , spalte );
		int sr = spin( (zeile + 1) % M , spalte );
		
		// Energieaenderung, wenn dieser Spin umgedreht wird:
		// Faktor 2: Spinaenderung um +- 2 ( +1 <-> -1) in jedem der 4 Paare
		int delta_e = 2 * spin(zeile, spalte) * (sd + su + sl + sr);
		
		// Relativer Boltzmann-Faktor bei Spinaenderung
		double r = exp(-delta_e * beta);
		
		// Metropolis-Entscheidung
		if ( r >= 1 || zufall() < r ){
			spin(zeile, spalte) = -spin(zeile, spalte);
			e += delta_e;
			m += 2*spin(zeile, spalte);
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
	
		// Mittelwerte berechnen (pro Spin
		double w = 1. / ( r * M*M * nSample );
		printf(" %5d %10.7g %10.7g\n", r, w*eSum, w*mSum);
	}
	
	finish();
}
    

