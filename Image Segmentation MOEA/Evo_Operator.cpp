#include "Evo_Operator.h"



Segment merge(Segment a, Segment b) {
		return Segment(a.get_image_ptr(),a,b);	
}