#include <glew.h>
#include <cstdlib>
#include "Particle.h"

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

Particle::Particle( float posX, float posY, float posZ, float minSize, float maxSize, int minLifetime, int maxLifetime, float floatingspeed, GLuint* tex) : posX(posX), posY(posY), posZ(posZ), floatingspeed(floatingspeed), tex(tex)
{
	size = RandomFloat(minSize, maxSize);
	this->lifetime = rand() % (maxLifetime - minLifetime + 1) + minLifetime;
}

void Particle::update()
{
	GLfloat sunLight[] = { 1,1,1};
	if(lifetime > 0)
	{
		if(gravity)
		{
			lifetime -= 0.1;

			if (discomode)
			{
				posY += floatingspeed / size;
			}
			else
			{
				posY -= floatingspeed / size;
			}
		}
		// do transformations
		// draw
		glPushMatrix();
		glTranslatef(posX, posY, posZ);
		drawGlowingSphere(sunLight);
		glPopMatrix();
	}
}

void Particle::drawGlowingSphere(GLfloat *glowColor) const
{

	GLUquadric *qobj = gluNewQuadric();

	gluQuadricTexture(qobj, GL_TRUE);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glMaterialfv(GL_FRONT, GL_EMISSION, glowColor);
	gluSphere(qobj, size, 10, 10);

	if (discomode) {
		GLfloat discomode[] = { 0.5 *static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - (-3)))), 0.5 * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - (-3)))), 0.5*static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - (-3)))) };
		glMaterialfv(GL_FRONT, GL_EMISSION, discomode);
	}
	else {
		GLfloat softSun[] = { 0.4, 0.4, 0.4 };//todo change to make snowy and variable
		glMaterialfv(GL_FRONT, GL_EMISSION, softSun);
	}
	gluDeleteQuadric(qobj);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}
