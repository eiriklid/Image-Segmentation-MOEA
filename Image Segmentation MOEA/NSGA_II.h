#pragma once

#include "Solution.h"
#include "Parameters.h"

struct Individual {
	Solution sol;
	int rank;
	double	crowdingDistance;
	std::vector<int> domminatingSolutionIDs;
	bool operator<( Individual& rhs) {
		if (rank != rhs.rank) return rank < rhs.rank;
		else return crowdingDistance > rhs.crowdingDistance;
		/*else {
			return DEVIATION_WEIGHT*sol.read_fitness(0) + EDGE_WEIGHT * sol.read_fitness(1) + CONNECTIVITY_WEIGHT * sol.read_fitness(2)
				< DEVIATION_WEIGHT*rhs.sol.read_fitness(0) + EDGE_WEIGHT * rhs.sol.read_fitness(1) + CONNECTIVITY_WEIGHT * rhs.sol.read_fitness(2);
		}*/
	}
	Individual() {}
	Individual(cv::Mat* image_ptr) {
		sol = Solution(image_ptr);
	}
	Individual(cv::Mat* image_ptr,double f1, double f2, double f3) {
		sol = Solution(image_ptr,f1,f2,f3);
	}

};

bool sortIndOnFitness1(Individual& lhs, Individual& rhs);
int display(std::vector<Individual>& poppulation, cv::Mat* image_ptr);
Individual* tourney(std::vector<Individual>* poppulation);
void crossover(const Individual* parent1, const Individual* parent2, Individual* child);
void mutate(Individual* ind);
void hard_mutate(Individual* ind);
void calcRank(std::vector<Individual>& poppulation);
bool sortOnDominatingSetSize(const Individual& lhs, const Individual& rhs);
void calcCrowdingDistance(std::vector<Individual>& poppulation);
bool sortOnFitness1(const Individual& lhs, const Individual& rhs);
bool sortOnFitness2(const Individual& lhs, const Individual& rhs);
bool sortOnFitness3(const Individual& lhs, const Individual& rhs);
bool sortSegmentsOnFitness1(const Segment& lhs, const Segment& rhs);
bool sortSegmentsOnFitness2(const Segment& lhs, const Segment& rhs);
bool sortSegmentsOnFitness3(const Segment& lhs, const Segment& rhs);

void NSGA_II(cv::Mat* image_ptr);