#pragma once
#include <glew.h>
#include <iostream>

class Particle
{
private:

	friend class ParticleSystem;

	float lifetime;

	float floatingspeed;

	float posX;
	float posY;
	float posZ;
	
	float size;

	bool discomode = false;
	bool gravity = true;

	GLuint* tex;

public:
	Particle(float posX, float posY, float posZ, float minSize, float maxSize, int minLifetime, int maxLifetime, float floatingspeed, GLuint* tex);
	void update();
	void setGravity(bool gravity)
	{
		this->gravity = gravity;
	}
	void setDiscomode(bool discomode)
	{
		this->discomode = discomode;
	}
	bool isAlive()
	{
		return lifetime > 0;
	}
	~Particle() = default;
private:
	void drawGlowingSphere(GLfloat *glowColor) const;
};
