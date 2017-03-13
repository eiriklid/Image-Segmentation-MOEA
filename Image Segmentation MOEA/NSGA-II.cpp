#include <vector>
#include "Parameters.h"
#include <iostream>

using namespace std;


struct Individual {
	Solution sol;
	int rank;
	double	crowdingDistance;
};

//Kjør samme parent child ting som jeg gjorde sist gang. 
//Sorter etter hvilken front de er i, 
//fronten som blir kuttet der børvi sortere etter niching strategi

//Selection: Tourney
//Vinner om man har bedre rank
//Vinner om man har bedre crowding distance ved samme rank.

//Crowding distance: Ytterpunktene på en front har uendelig
//Ellers. Finn det nærmeste elementet på hver front
//i_cd = sum over all fitness functions: (f[i+1]-f[i-1])/(f_max-f_min)
//Så skaler dem utifra min og maks verdi langs paretor fronte til den fitnessen

//Så basicly skal vi sortere poppulasjonen med foreldre og barn med hensyn på rank og så diversitet. 

Solution NSGA_II() {
	//0 -> POPPULATION_SIZE-1: Parents, POPPULATION_SIZE->2*POPPULATION_SIZE-1: Children
	vector<Individual> poppulation(2*POPPULATION_SIZE, /*Noe her*/);
	
	int cou
	while (1) {


		
	}
}