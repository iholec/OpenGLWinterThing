#include "Object.h"



Object::Object()
{
	//Position
	posX = -3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - (-3))));
	posY = -3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - (-3))));
	posZ = -3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - (-3))));

	//Rotation 1
	rotaX1 = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));
	rotaY1 = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));
	rotaZ1 = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));

	//Rotation 2
	rotaX2 = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));
	rotaY2 = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));
	rotaZ2 = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));

	
	size = 0.1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0.1)));
	rotaSpeed = 25 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (100 - 25)));
}



Object::~Object()
{
}
