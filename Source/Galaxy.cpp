#include "Galaxy.h"

Galaxy::Galaxy()
{
	//Position
	centerX = -25 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (25 - (-25))));
	centerY = -25 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (25 - (-25))));
	centerZ = -25 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (25 - (-25))));

	//Rotation
	rotaX = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));
	rotaY = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));
	rotaZ = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0)));
}
Galaxy::~Galaxy() {

}
