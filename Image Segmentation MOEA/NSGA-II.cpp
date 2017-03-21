#include <vector>
#include "Parameters.h"
#include <string>
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
	srand(time(NULL));
	//0 -> POPPULATION_SIZE-1: Parents, POPPULATION_SIZE->2*POPPULATION_SIZE-1: Children
	vector<Individual> poppulation(2 * POPPULATION_SIZE, Individual());
	
	//Initialiserer
	for (auto it = poppulation.begin(); it != poppulation.end(); ++it) {
		*it = Individual(image_ptr);
		it->sol.calc_fitness();
	}
	/*for (int i = 0; i < 2*POPPULATION_SIZE; ++i) {
		poppulation[i] = Individual(image_ptr, 1, 1, 1);
	}*/

	//Beregn rank og diversitet
	calcRank(poppulation);
	calcCrowdingDistance(poppulation);
	sort(poppulation.begin(), poppulation.end());

	//Variabler til loopen
	Individual* parent1;
	Individual* parent2;
	Individual* child;

	int count= 0;
	int num = 0;
	int printRate= 1;
	/*cout << "Number of itterations: ";
	cin >> num;
	cout << "\nPrint rate: ";
	cin >> printRate;*/
	int read;

	points_set_t edge;



	cout << "\n\n\n";

	while (count >= num) {
		num += display(poppulation, image_ptr);
	}

	while (1) {
		//Lag barn, plasser fra POPPULATION_SIZE->end
		for (int i = POPPULATION_SIZE; i < 2 * POPPULATION_SIZE; ++i) {
			child = &(poppulation[i]);
			parent1 = tourney(&poppulation);
			if (CLONE_RATE && !(rand() % CLONE_RATE)) {
				*child = *parent1;
				hard_mutate(child);
			}
			else {
				parent2 = tourney(&poppulation);
				crossover(parent1, parent2, child);
				mutate(child);
			}
			child->sol.calc_fitness();
		}

		//Beregn rank og diversitet
		calcRank(poppulation);
		calcCrowdingDistance(poppulation);

		//Sorter mhp rank så diversitet.
		sort(poppulation.begin(), poppulation.end());
		sort(poppulation.begin(), poppulation.begin()+POPPULATION_SIZE,sortIndOnFitness1);

		if (!(count%printRate)) {
			cout << count << ' ';
		}
		while (count >= num) {
			num += display(poppulation, image_ptr);
		}
		count++;
	}
}

int display(vector<Individual>& poppulation, cv::Mat* image_ptr) {
	int read;
	points_set_t edge;
	cout << "Highest rank: " << (poppulation.end() - 1)->rank << endl;
	
	for (int i = 0; i < POPPULATION_SIZE && poppulation[i].rank == poppulation[0].rank; ++i) {
		cout << endl << i << ") numSeg: " << poppulation[i].sol.segments.size() << "\tCD: " << poppulation[i].crowdingDistance;
		if(USE_OVERALL_DEVIATION) cout << "\tOD: " << poppulation[i].sol.read_fitness()[0];
		if(USE_EDGE_VALUE) cout << "\tEV: " << poppulation[i].sol.read_fitness()[1];
		if(USE_CONNECTIVITY_MEASURE) cout << "\tCM: " << poppulation[i].sol.read_fitness()[2];
		cout << endl;
	}

	cout << "Write index to see solution, negative number to continue\n";
	cin >> read;
	cout << endl << endl;
	if (read < 0) return -read;
	else {

		cv::Mat disp = image_ptr->clone();
		cv::Mat bw_disp = image_ptr->clone();
		bw_disp = cv::Scalar(255, 255, 255);
		for (seg_vec_t::iterator seg_it = poppulation[read].sol.segments.begin(); seg_it != poppulation[read].sol.segments.end(); ++seg_it) {
			seg_it->get_edge(&edge);
			for (points_set_t::iterator it = edge.begin(); it != edge.end(); it++) {
				disp.ptr<RGB>((*it).y)[(*it).x] = RGB(0, 255, 0);
				bw_disp.ptr<RGB>((*it).y)[(*it).x] = RGB(0, 0, 0);
			}
		}
		cv::namedWindow("NSGA window", cv::WINDOW_AUTOSIZE);// Create a window for display.
		cv::imshow("NSGA window", disp);                   // Show our image inside it.
		cv::namedWindow("NSGA BW window", cv::WINDOW_AUTOSIZE);// Create a window for display.
		cv::imshow("NSGA BW window", bw_disp);
		cv::waitKey(0);

	}
	return 0;
}


bool sortIndOnFitness1(Individual& lhs, Individual& rhs) {
	return lhs.sol.read_fitness(0) < rhs.sol.read_fitness(0);
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
	child->sol.segments.insert(child->sol.segments.begin(), parent2->sol.segments.begin(), parent2->sol.segments.end());

	for (auto it = child->sol.segments.begin(); it != child->sol.segments.end(); ++it) {
		it->isChanged = 0;
	}

	//Sorter segmentene hos barnet etter etellerannet
	typedef bool(*sortFunc)(const Segment&, const Segment&);
	vector<sortFunc> sortArray;
	if (USE_OVERALL_DEVIATION) sortArray.push_back(&sortSegmentsOnFitness1);
	if (USE_EDGE_VALUE) sortArray.push_back(&sortSegmentsOnFitness2);
	if (USE_CONNECTIVITY_MEASURE) sortArray.push_back(&sortSegmentsOnFitness3);

	sort(child->sol.segments.begin(), child->sol.segments.end(), sortArray[rand()%sortArray.size()]);
	//sort(child->sol.segments.begin(), child->sol.segments.end(), sortArray[0]);

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
	/*Point currentPoint;
	vector<Segment>::iterator newSegment;
	point_vec_t unhandled_neighbours;
	point_vec_t current_neighbours;
	for (int i = 0; i < child->sol.segments.size(); ++i) {
		if (child->sol.segments[i].isChanged) {
			//while det er piksler igjen i segmentet
			while (child->sol.segments[i].points.size() > 0) {
				//velg en tilfeldig piksel, putt den i ett nytt segment (valgte tilfeldighvis den første :P)
				currentPoint = *(child->sol.segments[i].points.begin());
				child->sol.segments[i].points.erase(currentPoint);
				child->sol.segments.push_back(Segment(child->sol.get_image_ptr(), currentPoint));
				newSegment = child->sol.segments.end() - 1;
				unhandled_neighbours.push_back(currentPoint);

				while (unhandled_neighbours.size()) {
					currentPoint = *(unhandled_neighbours.end()-1);
					unhandled_neighbours.pop_back();
					newSegment->points.insert(currentPoint);
					//finn alle naboene og putt dem i dette segmentet, slett fra det gamle
					current_neighbours = neighbours(currentPoint, child->sol.segments[i].get_image_ptr());
					for (auto point_it = current_neighbours.begin(); point_it != current_neighbours.end(); point_it++) {
						//hvis vi klarte å slette det var det i settet
						if (child->sol.segments[i].points.erase(*point_it)) {
							//når vi kommer til ett element vi har tatt før er det 
							//allerede slettet så det kan vi ikke gjøre igjen
							unhandled_neighbours.push_back(*point_it);
						}
					}

				}
			}
		}
	}*/
		
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
	if (ind->sol.segments.size()<MIN_NUM_SEGMENTS || MUTATION_SPLIT_RATE && !(rand() % MUTATION_SPLIT_RATE)) {
		ind->sol.mutation_split();
	}
	if (ind->sol.segments.size()>MAX_NUM_SEGMENTS || MUTATION_MERGE_RATE && !(rand() % MUTATION_MERGE_RATE)) {
		ind->sol.mutation_merge();
	}
}

void hard_mutate(Individual* ind) {
	if(rand()%2) {
		ind->sol.mutation_split();
	}
	else {
		ind->sol.mutation_merge();
	}
}

void calcRank(vector<Individual>& poppulation) {
	for (int i = 0; i < poppulation.size(); ++i) {
		poppulation[i].rank = 0;
	}

	for (int i = 0; i < poppulation.size(); ++i) {
		poppulation[i].domminatingSolutionIDs.clear();
		if (poppulation[i].rank == INT_MAX) continue;
		//Find all dominating individuals
		for (int j = 0; j < poppulation.size(); ++j) {
			if (i != j && poppulation[j].rank != INT_MAX) {
				if ((!USE_OVERALL_DEVIATION || poppulation[j].sol.read_fitness(0) == poppulation[i].sol.read_fitness(0))
					&& (!USE_EDGE_VALUE ||poppulation[j].sol.read_fitness(1) == poppulation[i].sol.read_fitness(1))
					&& (!USE_CONNECTIVITY_MEASURE || poppulation[j].sol.read_fitness(2) == poppulation[i].sol.read_fitness(2))) {
					poppulation[j].rank = INT_MAX;
				}
				else if ((!USE_OVERALL_DEVIATION || poppulation[j].sol.read_fitness(0) < poppulation[i].sol.read_fitness(0))
					&& (!USE_EDGE_VALUE || poppulation[j].sol.read_fitness(1) < poppulation[i].sol.read_fitness(1))
					&& (!USE_CONNECTIVITY_MEASURE || poppulation[j].sol.read_fitness(2) < poppulation[i].sol.read_fitness(2)) ) {
					poppulation[i].domminatingSolutionIDs.push_back(j);
				}
			}
		}
	}

	//Sort based on dominating solution size.
	sort(poppulation.begin(), poppulation.end(), sortOnDominatingSetSize);

	for (auto it = poppulation.begin(); it != poppulation.end(); ++it) {
		if (it->rank == INT_MAX) continue;
		if (it->sol.segments.size() < MIN_NUM_SEGMENTS || it->sol.segments.size() > MAX_NUM_SEGMENTS) {
			it->rank = INT_MAX;
		}
		else if (it->domminatingSolutionIDs.size() == 0) {
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
	sort(poppulation.begin(), poppulation.end());
	//Må sette alle crowding distansene til 0 her!!
	for (auto it = poppulation.begin(); it != poppulation.end(); ++it) {
		it->crowdingDistance = 0;
	}

	int rankEnd = 0;
	double f_min;
	double f_max;
	typedef bool(*sortFunc)(const Individual&, const Individual&);
	vector<sortFunc> sortArray;
	if(USE_OVERALL_DEVIATION) sortArray.push_back(&sortOnFitness1);
	if(USE_EDGE_VALUE) sortArray.push_back(&sortOnFitness2);
	if(USE_CONNECTIVITY_MEASURE) sortArray.push_back(&sortOnFitness3);
	for (int rankStart = 0; rankStart < poppulation.size();) {
		//find rank end
		while (rankEnd != poppulation.size() && poppulation[rankEnd].rank == poppulation[rankStart].rank) {
			rankEnd++;
		}


		for (int objFunc = 0; objFunc < sortArray.size(); ++objFunc) {
			//sorter [rankStart, rankEnd> mhp hver av fitness funksjonene
			sort(poppulation.begin() + rankStart, poppulation.begin() + rankEnd, sortArray[objFunc]);
			//Setter ytterpunktene i denne dimmensjonen til å ha uendelig crowdingDistance 
			poppulation[rankStart].crowdingDistance = INFINITY;
			poppulation[rankEnd - 1].crowdingDistance = INFINITY;
			f_min = poppulation[rankStart].sol.read_fitness_id(objFunc);
			f_max = poppulation[rankEnd - 1].sol.read_fitness_id(objFunc);

			if (rankStart + 1 < rankEnd - 1) {
				//mer enn 2 elementer
				for (auto it = poppulation.begin() + rankStart + 1; it != poppulation.begin() + rankEnd - 1; ++it) {
					it->crowdingDistance += ((it + 1)->sol.read_fitness_id(objFunc) - (it - 1)->sol.read_fitness_id(objFunc)) / (f_max - f_min);
				}
			}
		}
		rankStart = rankEnd;
	}
}

double Solution::read_fitness_id(int id) {
	if (!USE_OVERALL_DEVIATION) {
		return read_fitness(id + 1);
	}
	else if (!USE_EDGE_VALUE && id==1) {
		return read_fitness(2);
	}
	return read_fitness(id);
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
