# Image-Segmentation-MOEA

TODO:
- [x] Få tak i RGB for pixel
- [x] RGB-Distanse mellom piksler
- [x] Putte pixel i segment-set(Lage genotype)
- [x] Fitness for segment
	- [x] Find centroid(average)
	- [x] Find border-pixels
	- [x] For pixel find neighbour pixels
		- [x] dynamic? vector!
		- [x] make 4 & 8(n-boxses) neighbourhood
		- [x] Iterator for point_vec_t
- [?] Find neighbour-segments of segment
	- [x] Check if neighbour
	- [?] Get all neighbours
- [?] Initialize Solution
- [?] Mutation
	- [?] Merge
		- [x] put segment together
		- [x] Check for conectivity?
	- [?] Split
- [] Crossover
	- [] sort segment from best to worse
	- [] Fix "broken" segments
- [x] Fitness for Solution
	- [x] overall deviation
	- [x] edge value
	- [x] connectivity measure 

Lage egen types-fil?

Warning:
Ryktet sier at cv::Mat sin at() har dårlgi kjøretid!!!
de annbefaler istedet å hente ut en peker og så brue vanlig[]

VI burde prøve droppe å gjøre sqrt(), 

Jeg har brukt unordred_set::erase feil, den må ta inn itteratorer hos seg selv
så vi må gå gjennom alle verdiene og slette dem en av gangen.