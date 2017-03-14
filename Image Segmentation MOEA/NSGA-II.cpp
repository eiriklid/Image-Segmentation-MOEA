#include <vector>
#include "Parameters.h"
#include <iostream>
#include "Solution.h"
#include <stdlib.h>
#include <time.h>
#include <algorithm>

using namespace std;


struct Individual {
	Solution sol;
	int rank;
	double	crowdingDistance;
	Individual() { sol = Solution(); }
	bool operator<(const Individual& rhs) {
		if (rank != rhs.rank) return rank < rhs.rank;
		else return crowdingDistance > rhs.crowdingDistance;
	}
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
	srand(time(NULL));
	//0 -> POPPULATION_SIZE-1: Parents, POPPULATION_SIZE->2*POPPULATION_SIZE-1: Children
	vector<Individual> poppulation(2 * POPPULATION_SIZE, Individual());
	

	//Beregn rank ogdiversitet

	//Variabler til loopen
	Individual* parent1;
	Individual* parent2;
	Individual* child;

	while (1) {
		//Lag barn, plasser fra POPPULATION_SIZE->end
		for (int i = POPPULATION_SIZE; i < 2 * POPPULATION_SIZE; ++i) {
			child = &(poppulation[i]);
			parent1 = tourney(&poppulation);
			if (CLONE_RATE && !(rand() % CLONE_RATE)) {
				*child = *parent1;
			}
			else {
				parent2 = tourney(&poppulation);
				crossover(parent1, parent2, child);
			}
			mutate(child);
		}

		//Beregn rank og diversitet

		//Sorter mhp rank så diversitet.
		sort(poppulation.begin(), poppulation.end());

		
	}
}

Individual* tourney(const vector<Individual>* poppulation) {

}

void crossover(const Individual* parent1, const Individual* parent2, Individual* child) {

}

void mutate(Individual* ind) {

}