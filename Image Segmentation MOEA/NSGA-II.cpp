#include <vector>
#include "Parameters.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "NSGA_II.h"

using namespace std;

void NSGA_II(cv::Mat* image_ptr) {
	cv::Mat display;
	srand(time(NULL));
	//0 -> POPPULATION_SIZE-1: Parents, POPPULATION_SIZE->2*POPPULATION_SIZE-1: Children
	vector<Individual> poppulation(2 * POPPULATION_SIZE, Individual());
	
	//Initialiserer
	for (auto it = poppulation.begin(); it != poppulation.end(); ++it) {
		*it = Individual(image_ptr);
	}

	//Beregn rank og diversitet
	calcRank(poppulation);
	calcCrowdingDistance(poppulation);

	//Variabler til loopen
	Individual* parent1;
	Individual* parent2;
	Individual* child;

	int count= 0;
	int num;
	int printRate;
	cout << "Number of itterations: ";
	cin >> num;
	cout << "\nPrint rate: ";
	cin >> printRate;
	int read;

	points_set_t edge;

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

		if (count%printRate) {
			cout << num;
		}
		while (count >= num) {
			for (int i = 0; i < POPPULATION_SIZE && poppulation[i].rank == poppulation[0].rank;++i) {
				cout << i << ") numSeg: " << poppulation[i].sol.segments.size() << endl;
			}
			
			cout << "Write index to see solution, negative number to continue\n";
			cin >> read;
			if (read < 0) num += -read;
			else {
				display = *image_ptr;
				for (seg_vec_t::iterator seg_it = poppulation[read].sol.segments.begin(); seg_it != poppulation[read].sol.segments.end(); ++seg_it) {
					seg_it->get_edge(&edge);
					for (points_set_t::const_iterator it = edge.begin(); it != edge.end(); it++) {
						//cout << *it << endl;
						display.at<cv::Vec3b>(*it)[0] = 0;
						display.at<cv::Vec3b>(*it)[1] = 255;
						display.at<cv::Vec3b>(*it)[2] = 0;
					}
				}
				cv::namedWindow("Green window", cv::WINDOW_AUTOSIZE);// Create a window for display.
				cv::imshow("Green window", display);                   // Show our image inside it.
				cv::waitKey(0);
			}
			cout << "\n\n\n";
		}
		count++;
	}
}

Individual* tourney(vector<Individual>* poppulation) {
	Individual* winner = &(*poppulation)[rand() % POPPULATION_SIZE];

	//sjangse for at _ikke_ tilfeldig velges
	if (!TOURNEY_ERROR_RATE || (rand() % TOURNEY_ERROR_RATE)) {
		vector<int> participants(TOURNEY_SIZE, 0);
		for (int i = 0; i < TOURNEY_SIZE; ++i) {
			participants.push_back(rand() % POPPULATION_SIZE);
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
	//HEAVY!

	//Put alle segmentene fra begge foreldrene inn hos barnet
	child->sol.segments.clear();
	child->sol.segments.insert(child->sol.segments.begin(), parent1->sol.segments.begin(), parent1->sol.segments.end());
	child->sol.segments.insert(child->sol.segments.begin(), parent1->sol.segments.begin(), parent1->sol.segments.end());

	for (auto it = child->sol.segments.begin(); it != child->sol.segments.end(); ++it) {
		it->isChanged = 0;
	}

	//Sorter segmentene hos barnet etter etellerannet
	typedef bool(*sortFunc)(const Segment&, const Segment&);
	sortFunc sortArray[3];
	sortArray[0] = &sortSegmentsOnFitness1;
	sortArray[1] = &sortSegmentsOnFitness2;
	sortArray[2] = &sortSegmentsOnFitness3;

	sort(child->sol.segments.begin(), child->sol.segments.end(), sortArray[rand()%3]);


	//Itterer gjennom alle segmentene i
	for (auto it = child->sol.segments.begin(); it != child->sol.segments.end(); ++it) {
		//Gå gjennomm alle etterliggende set j
		for (auto jt = it + 1; jt != child->sol.segments.end(); ++jt) {
			//slett i fra j
			jt->isChanged |= jt->erase(*it);
			//Marker om noe ble endret
		}
	}
		

	//HEAVY, TODO dette må det gå ann å gjøre bedre, eller teste på mer før vi deler.
	//Utvikl noen tester for å se om den ble delt, sett isChanged om det er ett problem
	//For alle segmenter som ble endret
	//TODO er dette rett??
	cv::Point2i currentPoint;
	vector<Segment>::iterator newSegment;
	point_vec_t unhandled_neighbours;
	point_vec_t current_neighbours;
	for (auto it = child->sol.segments.begin(); it != child->sol.segments.end(); ++it) {
		if (it->isChanged) {
			//while det er piksler igjen i segmentet
			while (it->points.size() > 0) {
				//velg en tilfeldig piksel, putt den i ett nytt segment (valgte tilfeldighvis den første :P)
				currentPoint = *(it->points.begin());
				it->points.erase(currentPoint);
				child->sol.segments.push_back(Segment(it->get_image_ptr(), currentPoint));
				newSegment = child->sol.segments.end() - 1;
				unhandled_neighbours.push_back(currentPoint);

				while (unhandled_neighbours.size()) {
					currentPoint = *(unhandled_neighbours.end()-1);
					unhandled_neighbours.pop_back();
					newSegment->points.insert(currentPoint);
					//finn alle naboene og putt dem i dette segmentet, slett fra det gamle
					current_neighbours = neighbours(currentPoint, it->get_image_ptr());
					for (auto point_it = current_neighbours.begin(); point_it != current_neighbours.end(); point_it++) {
						//hvis vi klarte å slette det var det i settet
						if (it->points.erase(*point_it) == 1) {
							//når vi kommer til ett element vi har tatt før er det 
							//allerede slettet så det kan vi ikke gjøre igjen
							unhandled_neighbours.push_back(*point_it);
						}
					}

				}
			}
		}
	}
		
	//Gå gjennom settene
	for (auto it = child->sol.segments.begin(); it != child->sol.segments.end();) {
		//Slett alle tomme.
		if (it->points.size() == 0) {
			it = child->sol.segments.erase(it);
		}
		else {
			++it;
		}
	}
}

bool sortSegmentsOnFitness1(const Segment& lhs, const Segment& rhs) {
	return lhs.read_overall_deviation()< rhs.read_overall_deviation();
}
bool sortSegmentsOnFitness2(const Segment& lhs, const Segment& rhs) {
	return lhs.read_edge_value() < rhs.read_edge_value();
}
bool sortSegmentsOnFitness3(const Segment& lhs, const Segment& rhs) {
	return lhs.read_connectivity_measure()< rhs.read_connectivity_measure();
}



void mutate(Individual* ind) {
	if (MUTATION_SPLIT_RATE && !(rand() % MUTATION_SPLIT_RATE)) {
		ind->sol.mutation_split();
	}
	if (MUTATION_MERGE_RATE && !(rand() % MUTATION_MERGE_RATE)) {
		ind->sol.mutation_merge();
	}
}

void calcRank(vector<Individual>& poppulation) {
	for (int i = 0; i < poppulation.size(); ++i) {
		poppulation[i].domminatingSolutionIDs.clear();
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
			it->rank = 0;
			for (auto dt = it->domminatingSolutionIDs.begin(); dt != it->domminatingSolutionIDs.end(); ++dt) {
				it->rank = max(it->rank, poppulation[*dt].rank+1);
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
	return lhs.sol.read_fitness(0) < rhs.sol.read_fitness(0);
}
bool sortOnFitness2(const Individual& lhs, const Individual& rhs) {
	return lhs.sol.read_fitness(1) < rhs.sol.read_fitness(1);
}
bool sortOnFitness3(const Individual& lhs, const Individual& rhs) {
	return lhs.sol.read_fitness(2) < rhs.sol.read_fitness(2);
}
