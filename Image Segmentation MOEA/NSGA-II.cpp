#include <vector>
#include "Parameters.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#include "NSGA_II.h"

using namespace std;

Solution NSGA_II() {
	srand(time(NULL));
	//0 -> POPPULATION_SIZE-1: Parents, POPPULATION_SIZE->2*POPPULATION_SIZE-1: Children
	vector<Individual> poppulation(2 * POPPULATION_SIZE, Individual());
	

	//Beregn rank og diversitet
	calcRank(poppulation);
	calcCrowdingDistance(poppulation);

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
		calcRank(poppulation);
		calcCrowdingDistance(poppulation);

		//Sorter mhp rank så diversitet.
		sort(poppulation.begin(), poppulation.end());		
	}
}

Individual* tourney(vector<Individual>* poppulation) {
	Individual* winner = &(*poppulation)[rand() % POPPULATION_SIZE];

	//sjangse for at ikke tilfeldig velges
	if (!TOURNEY_ERROR_RATE || (rand() % TOURNEY_ERROR_RATE)) {

		vector<int> participants(TOURNEY_SIZE, 0);
		for (int i = 0; i < TOURNEY_SIZE; ++i) {
			participants[i] = rand() % POPPULATION_SIZE;
		}

		for (int i = 0; i < TOURNEY_SIZE; ++i) {
			if ((*poppulation)[participants[i]] < *winner) {
				winner = &(*poppulation)[participants[i]];
			}
		}
	}
	return winner;
}

void crossover(const Individual* parent1, const Individual* parent2, Individual* child) {

}

void mutate(Individual* ind) {

}

void calcRank(vector<Individual>& poppulation) {
	for (int i = 0; i < poppulation.size(); ++i) {
		poppulation[i].domminatingSolutionIDs.clear(); //HEAVY
		//Find all dominating individuals
		for (int j = 0; j < poppulation.size(); ++j) {
			if (i != j) {
				if (poppulation[j].sol.read_fitness(0) < poppulation[i].sol.read_fitness(0)
					&& poppulation[j].sol.read_fitness(1) < poppulation[i].sol.read_fitness(1)
					&& poppulation[j].sol.read_fitness(2) < poppulation[i].sol.read_fitness(2)) {
					poppulation[i].domminatingSolutionIDs.push_back(j);
				}
			}
		}
	}

	//Sort based on dominating solution size.
	sort(poppulation.begin(), poppulation.end(), sortOnDominatingSetSize);

	for (auto it = poppulation.begin(); it != poppulation.end(); ++it) {
		if (it->domminatingSolutionIDs.size() == 0) {
			it->rank = 0;
		}
		else {
			//rank = highest rank of dominators +1
			it->rank = INT_MAX;
			for (auto dt = it->domminatingSolutionIDs.begin(); dt != it->domminatingSolutionIDs.end(); ++dt) {
				it->rank = min(it->rank, poppulation[*dt].rank+1);
			}
		}
	}
}

bool sortOnDominatingSetSize(const Individual& lhs, const Individual& rhs) {
	return lhs.domminatingSolutionIDs.size() < rhs.domminatingSolutionIDs.size();
}


//Calcullate rank first
void calcCrowdingDistance(vector<Individual>& poppulation) {
	//Må sette alle crowding distansene til 0 her!!
	for (auto it = poppulation.begin(); it != poppulation.end(); ++it) {
		it->crowdingDistance = 0;
	}

	int rankEnd = 0;
	double f_min;
	double f_max;
	typedef bool(*sortFunc)(const Individual&, const Individual&);
	sortFunc sortArray[3];
	sortArray[0] = &sortOnFitness1;
	sortArray[1] = &sortOnFitness2;
	sortArray[2] = &sortOnFitness3;
	for (int rankStart = 0; rankStart < poppulation.size();) {
		//find rank end
		while (rankEnd != poppulation.size() && poppulation[rankEnd].rank == poppulation[rankStart].rank) {
			rankEnd++;
		}


		for (int objFunc = 0; objFunc < NUM_OBJECTIVES; ++objFunc) {
			//sorter [rankStart, rankEnd> mhp hver av fitness funksjonene
			sort(poppulation.begin() + rankStart, poppulation.end() + rankEnd, sortArray[objFunc]);
			//Setter ytterpunktene i denne dimmensjonen til å ha uendelig crowdingDistance 
			poppulation[rankStart].crowdingDistance = INFINITY;
			poppulation[rankEnd - 1].crowdingDistance = INFINITY;
			f_min = poppulation[rankStart].sol.read_fitness(objFunc);
			f_max = poppulation[rankEnd - 1].sol.read_fitness(objFunc);

			if (rankStart + 1 < rankEnd - 1) {
				//mer enn 2 elementer
				for (auto it = poppulation.begin() + rankStart + 1; it != poppulation.begin() + rankEnd - 1; ++it) {
					it->crowdingDistance += ((it + 1)->sol.read_fitness(objFunc) - (it - 1)->sol.read_fitness(objFunc)) / (f_max - f_min);
				}
			}
		}
		rankStart = rankEnd;
	}
}

bool sortOnFitness1(const Individual& lhs, const Individual& rhs) {
	return lhs.sol.read_fitness(0) < lhs.sol.read_fitness(0);
}
bool sortOnFitness2(const Individual& lhs, const Individual& rhs) {
	return lhs.sol.read_fitness(1) < lhs.sol.read_fitness(1);
}
bool sortOnFitness3(const Individual& lhs, const Individual& rhs) {
	return lhs.sol.read_fitness(2) < lhs.sol.read_fitness(2);
}
