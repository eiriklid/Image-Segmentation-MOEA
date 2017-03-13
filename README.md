# Image-Segmentation-MOEA

TODO:
	- [x] Få tak i RGB for pixel
	- [x] RGB-Distanse mellom piksler
	- [x] Putte pixel i segment-set(Lage genotype)
	- [] Fitness for segment
		- [] Find centroid
		- [] Find border-pixels
			- Max/Min for each row?
			- 2D-Array with Max/Min?
		- [x] For pixel find neighbour pixels
			-struct with directions of neighbour?
			-dynamic?
	- [] Inserting pixel 
		- Check if pixel connects to segment
		- Uppdate min/max or edges list
	- [] Erasing pixel
		- Check if pixel makes hole in segment?
			- Make inner rim
		- Check if pixel is min/max?
			- If both Min & Max, check neighbours to detect split. (or Check that the outer rim rows is continious)
	
	- [] Detect splits
		- check for erase inside outer rim

	