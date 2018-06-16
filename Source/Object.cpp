#include "Object.h"



Object::Object()
{
	//Position
	posX = -5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5 - (-5))));
	posY = -5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5 - (-5))));
	posZ = -5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5 - (-5))));

	//Rotation 1
	rotaX1 = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));
	rotaY1 = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));
	rotaZ1 = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));

	//Rotation 2
	rotaX2 = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));
	rotaY2 = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));
	rotaZ2 = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));

	
	size = 0.1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0.1)));
	rotaSpeed = 5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (100 - 5)));
}



Object::~Object()
{
}
