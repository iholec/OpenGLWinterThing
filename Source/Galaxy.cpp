#include "Galaxy.h"

Galaxy::Galaxy()
{
	//Position
	centerX = -15 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (15 - (-15))));
	centerY = -15 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (15 - (-15))));
	centerZ = -15 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (15 - (-15))));
	size = rand() % 10;
}
Galaxy::~Galaxy() {

}
