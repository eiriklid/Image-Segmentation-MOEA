#pragma once

#include "Solution.h"

struct Individual {
	Solution sol;
	int rank;
	double	crowdingDistance;
	std::vector<int> domminatingSolutionIDs;
	bool operator<(const Individual& rhs) {
		if (rank != rhs.rank) return rank < rhs.rank;
		else return crowdingDistance > rhs.crowdingDistance;
	}
	Individual() {}
	Individual(cv::Mat* image_ptr) {
		sol = Solution(image_ptr);
	}

};

Individual* tourney(std::vector<Individual>* poppulation);
void crossover(const Individual* parent1, const Individual* parent2, Individual* child);
void mutate(Individual* ind);
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