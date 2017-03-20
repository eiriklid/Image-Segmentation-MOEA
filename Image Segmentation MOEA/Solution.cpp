#include "Segment.h"
#include "Solution.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>


using namespace std;

Solution::Solution() {

}

//Initializes to one segment per pixel
Solution::Solution(cv::Mat* image_ptr){
	//SPAGHETTI level: low
	//segments = seg_vec_t(image_ptr->cols*image_ptr->rows);
	segments.reserve(image_ptr->cols*image_ptr->rows/25);
	this->image_ptr = image_ptr;
	int x_seg;
	int y_seg;
	int y_tot = 0;
	do 
	{
		int x_tot = 0;
		y_seg = ((rand() %( image_ptr->rows / 5)) + y_tot)+1;
		if (y_seg >= image_ptr->rows) {
			y_seg = image_ptr->rows - 1;
		}
		do {
			x_seg = (rand() %( image_ptr->cols / 5)) + x_tot + 1;
			if (x_seg >= image_ptr->cols) {
				x_seg = image_ptr->cols - 1;
			}
			Segment temp_seg(image_ptr);
			for (int i = x_tot; i < x_seg; i++) {
				for (int j = y_tot; j < y_seg; j++) {
					temp_seg.insert_pixel(cv::Point2i(i, j));
				}
			}
			segments.push_back(temp_seg);
			x_tot = x_seg;
				

		} while (x_tot < image_ptr->cols - 1);
		
	y_tot = y_seg;

	} while (y_tot < image_ptr->rows-1);
	/*	
	for (int i = 0; i < image_ptr->cols; i++) {
		for (int j = 0; j < image_ptr->rows; j++) {
			segments.push_back(Segment(image_ptr, cv::Point2i(i, j)));
			(segments.end() - 1)->average = image_ptr->ptr<RGB>(j)[i];
		}
	}
	*/
	//Nå har vi ett segment for hver piksel,
	//Velg en tilfeldig piksel og merge.
	
	Segment* seg;
	unordered_set<int> neighbours;
	while (segments.size() > INIT_MIN_NUN_SEGMENTS && (segments.size() > INIT_MAX_NUM_SEGMENTS || rand() % (INIT_MAX_NUM_SEGMENTS - INIT_MIN_NUN_SEGMENTS))){
		seg = &*(segments.begin() + rand() % segments.size());
		find_neighbours(seg, &segments, &neighbours,image_ptr);
		merge(seg,neighbours);
	}
	
}
const seg_vec_t* Solution::get_segments() {
	return &segments;
}

void Solution::calc_fitness() {
	fitness[0] = 0;
	fitness[1] = 0;
	fitness[2] = 1;

	for (auto it = segments.begin(); it != segments.end(); ++it) {
		fitness[0] += it->calc_conectivity_measure();
		fitness[1] += it->calc_edge_value();
		fitness[2] += it->calc_overall_deviation();
	}
}

void find_neighbours(const Segment* seg, vector<Segment>* segments, unordered_set<int>* neighbourIDs, cv::Mat* image_ptr) {
	//SPAGHETTI level: low
	neighbourIDs->clear();
	points_set_t neighbouring_pixels;
	seg->get_neighbouring_pixels(&neighbouring_pixels);

	for (auto neighbour_it = neighbouring_pixels.begin(); neighbour_it != neighbouring_pixels.end(); neighbour_it++) {
		//if its not in our segment, find a segment where it is
		if (seg->points.find(*neighbour_it) == seg->points.end()) {
			for (int segmentID = 0; segmentID != segments->size(); ++segmentID) {
				//if we find the list where the pixel belongs
				if (segments->at(segmentID).points.find(*neighbour_it) != segments->at(segmentID).points.end()) {
					//blir ikke lagt inn dobbelt pga unordred set
					neighbourIDs->insert(segmentID);
					break;
				}
			}
		}
	}
}

//identisk den over men med en peker innen segments.
void find_neighbours(const Segment* seg, std::vector<Segment*>* segments, std::unordered_set<int>* neighbourIDs, cv::Mat* image_ptr) {
	//SPAGHETTI level: low
	neighbourIDs->clear();
	points_set_t neighbouring_pixels;
	seg->get_neighbouring_pixels(&neighbouring_pixels);

	for (auto neighbour_it = neighbouring_pixels.begin(); neighbour_it != neighbouring_pixels.end(); neighbour_it++) {
		//if its not in our segment, find a segment where it is
		if (seg->points.find(*neighbour_it) == seg->points.end()) {
			for (int segmentID = 0; segmentID != segments->size(); ++segmentID) {
				//if we find the list where the pixel belongs
				if (segments->at(segmentID)->points.find(*neighbour_it) != segments->at(segmentID)->points.end()) {
					//blir ikke lagt inn dobbelt pga unordred set
					neighbourIDs->insert(segmentID);
					break;
				}
			}
		}
	}
}


//Merge seg1 with one of the given neighbourIDs, where it is the ID in the local variable segments
/*int Solution::merge(Segment* seg1, unordered_set<int>& neighbourIDs_segments) {
	//SPAGHETTI level: medium-
	double originalFitness[NUM_OBJECTIVES];
	originalFitness[0] = seg1->read_overall_deviation();
	originalFitness[1] = seg1->read_edge_value();
	originalFitness[2] = seg1->read_connectivity_measure();

	auto bestNeighbour_it = neighbourIDs_segments.begin();
	double bestNeighbourVal = INFINITY;
	double currentVal;
	for (auto it = neighbourIDs_segments.begin(); it != neighbourIDs_segments.end(); ++it) {
		//merger
		seg1->insert(segments[*it]);
		//Sjekker hvor bra det er 
		seg1->calc_fitness();
		//NB her er det andre måter å regne dette på
		//1) Velg en tilfeldig objective function
		//2) Velg tilfedig skalerings konstant
		//3) normaliser dem mhp på originalverdien til fitnessene
		currentVal = seg1->read_overall_deviation() / originalFitness[0] +
			seg1->read_edge_value() / originalFitness[1] +
			seg1->read_connectivity_measure() / originalFitness[2];
		if (currentVal < bestNeighbourVal) {
			bestNeighbour_it = it;
			bestNeighbourVal = currentVal;
		}
		//Sletter den igjen
		seg1->erase(segments[*it]);
	}
	//merger den som faktisk var best
	seg1->insert(segments[*bestNeighbour_it]);
	//Sletter den vi merget fra
	segments.erase(segments.begin() + *bestNeighbour_it);
	return *bestNeighbour_it;
}


//Merge seg1 with one of the given neighbourIDs, where the ID reffers to the given set
//delets the merged element from Solution::segments and the given list segment_pts
int Solution::merge(Segment* seg1, vector<Segment*>* segment_pts, unordered_set<int>& neighbourIDs) {
	//SPAGHETTI level: medium+
	double originalFitness[NUM_OBJECTIVES];
	originalFitness[0] = seg1->read_overall_deviation();
	originalFitness[1] = seg1->read_edge_value();
	originalFitness[2] = seg1->read_connectivity_measure();

	auto bestNeighbour_it = neighbourIDs.begin();
	double bestNeighbourVal = INFINITY;
	double currentVal;
	for (auto it = neighbourIDs.begin(); it != neighbourIDs.end(); ++it) {
		//merger
		seg1->insert(*(segment_pts->at(*it)));
		//Sjekker hvor bra det er 
		seg1->calc_fitness();
		//NB her er det andre måter å regne dette på
		//1) Velg en tilfeldig objective function
		//2) Velg tilfedig skalerings konstant
		//3) normaliser dem mhp på originalverdien til fitnessene
		currentVal = seg1->read_overall_deviation() / originalFitness[0] +
			seg1->read_edge_value() / originalFitness[1] +
			seg1->read_connectivity_measure() / originalFitness[2];
		if (currentVal < bestNeighbourVal) {
			bestNeighbour_it = it;
			bestNeighbourVal = currentVal;
		}
		//Sletter den igjen
		seg1->erase(*(segment_pts->at(*it)));
	}
	//merger den som faktisk var best
	seg1->insert(*(segment_pts->at(*bestNeighbour_it)));
	//Sletter den vi merget fra
	//Må også finne den i Solution::segments og slette den der.
	auto pos_SolutionSegments = find(segments.begin(), segments.end(),*(segment_pts->at(*bestNeighbour_it)));


	//Sletter fra lista over segments vi nå ser på
	segment_pts->erase(segment_pts->begin() + *bestNeighbour_it);

	return *bestNeighbour_it;
}*/

//trenger å starte med segmenter som har rett average color, resultatet har fortsatt rett average color
void Solution::merge(Segment* seg1, vector<Segment*>* segment_pts, unordered_set<int>& neighbourIDs) {

	//SPAGHETTI level: medium+
	Segment* bestNeighbour = seg1;
	unordered_set<int>::iterator bestNeighbour_it;
	double bestNeighbourVal = INFINITY;
	double currentVal;
	for (auto it = neighbourIDs.begin(); it != neighbourIDs.end(); ++it) {
		currentVal = color_distance(segment_pts->at(*it)->average, seg1->average);
		if (currentVal < bestNeighbourVal) {
			bestNeighbourVal = currentVal;
			bestNeighbour = segment_pts->at(*it);
			bestNeighbour_it = it;
		}
	}

	if (bestNeighbour == seg1) return;

	//merger den som faktisk var best
	seg1->insert(*bestNeighbour);

	//sett average
	seg1->average.blue = ((double)seg1->average.blue * seg1->points.size() + (double)bestNeighbour->average.blue * bestNeighbour->points.size()) / (seg1->points.size() + bestNeighbour->points.size());
	seg1->average.green = ((double)seg1->average.green * seg1->points.size() + (double)bestNeighbour->average.green * bestNeighbour->points.size()) / (seg1->points.size() + bestNeighbour->points.size());
	seg1->average.red= ((double)seg1->average.red * seg1->points.size() + (double)bestNeighbour->average.red * bestNeighbour->points.size()) / (seg1->points.size() + bestNeighbour->points.size());

	//Sletter den vi merget fra
	//Må også finne den i Solution::segments og slette den der.
	auto pos_SolutionSegments = find(segments.begin(), segments.end(), *bestNeighbour);
	segments.erase(pos_SolutionSegments);

	//Sletter fra lista over segments vi nå ser på
	segment_pts->erase(segment_pts->begin()+*bestNeighbour_it);
}
void Solution::merge(Segment* seg1, unordered_set<int>& neighbourIDs) {
	//SPAGHETTI level: medium+
	Segment* bestNeighbour = seg1;
	unordered_set<int>::iterator bestNeighbour_it;
	double bestNeighbourVal = INFINITY;
	double currentVal;
	for (auto it = neighbourIDs.begin(); it != neighbourIDs.end(); ++it) {
		currentVal = color_distance(segments.at(*it).average, seg1->average);
		if (currentVal < bestNeighbourVal) {
			bestNeighbourVal = currentVal;
			bestNeighbour = &(segments.at(*it));
			bestNeighbour_it = it;
		}
	}


	if (bestNeighbour == seg1) {
		return;
	}

	//merger den som faktisk var best
	seg1->insert(*bestNeighbour);

	//sett average
	seg1->average.blue = ((double)seg1->average.blue * seg1->points.size() + (double)bestNeighbour->average.blue * bestNeighbour->points.size()) / (seg1->points.size() + bestNeighbour->points.size());
	seg1->average.green = ((double)seg1->average.green * seg1->points.size() + (double)bestNeighbour->average.green * bestNeighbour->points.size()) / (seg1->points.size() + bestNeighbour->points.size());
	seg1->average.red = ((double)seg1->average.red * seg1->points.size() + (double)bestNeighbour->average.red * bestNeighbour->points.size()) / (seg1->points.size() + bestNeighbour->points.size());

	//Sletter den vi merget fra
	segments.erase(segments.begin() + *bestNeighbour_it);
}

void Solution::split(seg_vec_t::iterator seg_it) {
	/*Splits innto little pieces, combines them uitill there are only two segments*/


	//Allokerer vektoren stor nok slik at størelsen alrdi endres,
	int numPixels = seg_it->points.size();
	vector<Segment*> toBeMerged;
	toBeMerged.reserve(numPixels);
	//Split i enkeltpiksler, skriv ned adressen i to be merged
	for (auto it = seg_it->points.begin(); it != seg_it->points.end(); ++it) {
		//legg til pikselen som eget segment
		segments.push_back(Segment(image_ptr, *it));

		//skriv ned at vi må ta hensyn til dette segmentet
		toBeMerged.push_back(&*(segments.end() - 1));
		(*(toBeMerged.end()-1))->average = image_ptr->ptr<RGB>(it->y)[it->x];
	}
	//sletter seg
	segments.erase(seg_it);

	//Merge sammen til vi har to segmenter
	Segment* current;
	unordered_set<int> neighbourIDs;
	while (toBeMerged.size() > 2) {
		//Velg en tilfeldig
		current = toBeMerged[rand() % toBeMerged.size()];
		//Finnn naboene som er blant toBeMerged
		find_neighbours(current, &toBeMerged, &neighbourIDs, image_ptr);
		//merge dem
		//Pass på at ting har beregnet average først
		merge(current, &toBeMerged,neighbourIDs);
	}
}



void Solution::mutation_merge() {
	//SPAGHETTI level: low
	//velg en tilfeldig
	Segment* seg1 = &*(segments.begin()+rand()%segments.size());

	//merg den med den beste naboen
	unordered_set<int> neighbourIDs;
	find_neighbours(seg1, &segments, &neighbourIDs, image_ptr);
	//Pass på at ting har regnet ut average først
	seg1->calc_average();
	for (auto it = neighbourIDs.begin(); it != neighbourIDs.end(); ++it) {
		segments[(*it)].calc_average();
	}

	merge(seg1, neighbourIDs);
}

void Solution::mutation_split() {
	//SPAGHETTI level: low
	split((segments.begin() + rand() % segments.size()));
}