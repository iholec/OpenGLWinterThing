#include "Galaxy.h"

Galaxy::Galaxy()
{
	//Position
	centerX = -15 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (15 - (-15))));
	centerY = -15 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (15 - (-15))));
	centerZ = -15 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (15 - (-15))));

	//Rotation
	rotaX = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));
	rotaY = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));
	rotaZ = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));
}
Galaxy::~Galaxy() {

}
