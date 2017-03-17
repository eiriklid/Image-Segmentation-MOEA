#include "Solution.h"
#include "Segment.h

#include <iostream>

using namespace std;

Solution::Solution() {

}


//Initializes to one segment per pixel
Solution::Solution(cv::Mat* image_ptr){
	segments = seg_vec_t(image_ptr->cols*image_ptr->rows);
	this->image_ptr = image_ptr;
	for (int i = 0; i < image_ptr->cols; i++) {
		for (int j = 0; j < image_ptr->rows; j++) {
			Segment seg = Segment(image_ptr, cv::Point2i(i, j));
			segments.push_back(seg);
		}
	}

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
/*
seg_ptr_vec_t Solution::neighbour_seg(Segment seg) {
	seg_ptr_vec_t neighbour_segs;
	for (seg_vec_t::iterator it = segments.begin(); it != segments.end(); it++) {
		if (seg.neighbour(*it)) {
			neighbour_segs.push_back(&*it);
		}
	}
	return neighbour_segs;
}


index_vec_t Solution::neighbour_seg(Segment seg) {
	seg_itr_vec_t neighbour_segs;
	for (seg_vec_t::iterator it = segments.begin(); it != segments.end(); it++) {
		if (seg.neighbour(*it)) {
			neighbour_segs.push_back(it);
		}
	}
	return neighbour_segs;
}
*/


void Solution::calc_fitness() {
	for (auto it = segments.begin(); it != segments.end(); ++it) {
		fitness[0] += it->calc_conectivity_measure();
		fitness[1] += it->calc_edge_value();
		fitness[2] += it->calc_overall_deviation();
	}
}

void find_neighbours(const Segment* seg, vector<Segment>* segments, unordered_set<int>* neighbourIDs, cv::Mat* image_ptr) {
	neighbourIDs->clear();
	points_set_t edges = seg->get_edge();
	point_vec_t neighbour_vec;
	for (points_set_t::const_iterator it = edges.begin(); it != edges.end(); it++) {
		neighbour_vec = neighbours(*it, image_ptr);
		for (point_vec_t::iterator neighbour_it = neighbour_vec.begin(); neighbour_it != neighbour_vec.end(); neighbour_it++) {
			//if its not in our segment, find a segment where it is
			if (seg->points.find(*neighbour_it) == seg->points.end()) {
				for (int segmentID = 0; segmentID != segments->size(); ++segmentID) {
					//if we find the list where the pixel belongs
					if (segments->at(segmentID).points.find(*neighbour_it) != segments->at(segmentID).points.end()) {
						neighbourIDs->insert(segmentID);
						break;
					}
				}
			}
		}
	}
}

//identisk den over men med en peker innen segments.
void find_neighbours(const Segment* seg, std::vector<Segment*>* segments, std::unordered_set<int>* neighbourIDs, cv::Mat* image_ptr) {
	neighbourIDs->clear();
	points_set_t edges = seg->get_edge();
	point_vec_t neighbour_vec;
	for (points_set_t::const_iterator it = edges.begin(); it != edges.end(); it++) {
		neighbour_vec = neighbours(*it, image_ptr);
		for (point_vec_t::iterator neighbour_it = neighbour_vec.begin(); neighbour_it != neighbour_vec.end(); neighbour_it++) {
			//if its not in our segment, find a segment where it is
			if (seg->points.find(*neighbour_it) == seg->points.end()) {
				for (int segmentID = 0; segmentID != segments->size(); ++segmentID) {
					//if we find the list where the pixel belongs
					if (segments->at(segmentID)->points.find(*neighbour_it) != segments->at(segmentID)->points.end()) {
						neighbourIDs->insert(segmentID);
						break;
					}
				}
			}
		}
	}
}


void Solution::split(seg_vec_t::iterator seg_it) {
	//Allokerer vektoren stor nok slik at størelsen alrdi endres,
	int numPixels = seg_it->points.size();
	vector<Segment*> toBeMerged;
	toBeMerged.reserve(numPixels);
	//Split i enkeltpiksler, skriv ned adressen i to be merged
	for (auto it = seg_it->points.begin(); it != seg_it->points.end(); ++it) {
		//legg til pikselen som eget segment
		segments.push_back(Segment(image_ptr,*it));
		//skriv ned at vi må ta hensyn til dette segmentet
		toBeMerged.push_back(&*(segments.end() - 1));
	}
	//sletter seg
	segments.erase(seg_it);

	//Merge sammen til vi har to segmenter
	Segment* current;
	unordered_set<int> neighbourIDs;
	int deletedID;
	while (toBeMerged.size() > 2) {
		//Velg en tilfeldig
		current = toBeMerged[rand() % toBeMerged.size()];
		//Finnn naboene som er blant toBeMerged
		find_neighbours(current, &toBeMerged, &neighbourIDs,image_ptr);
		//merge dem
		deletedID = merge(current, neighbourIDs);
		//Her må vi gjøre et eller annet med toBeMerged.
		toBeMerged.erase(toBeMerged.begin() + deletedID);
	}
}


int Solution::merge(Segment* seg1, unordered_set<int>& neighbourIDs) {
	double originalFitness[NUM_OBJECTIVES];
	originalFitness[0] = seg1->read_overall_deviation();
	originalFitness[1] = seg1->read_edge_value();
	originalFitness[2] = seg1->read_connectivity_measure();

	auto bestNeighbour_it = neighbourIDs.begin();
	double bestNeighbourVal = INFINITY;
	double currentVal;
	for (auto it = neighbourIDs.begin(); it != neighbourIDs.end(); ++it) {
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


void Solution::mutation_merge() {
	//velg en tilfeldig
	Segment* seg1 = &*(segments.begin()+rand()%segments.size());

	//merg den med den beste naboen
	unordered_set<int> neighbourIDs;
	find_neighbours(seg1, &segments, &neighbourIDs, image_ptr);
	merge(seg1, neighbourIDs);
}

void Solution::mutation_split() {
	split((segments.begin() + rand() % segments.size()));
}