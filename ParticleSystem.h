#pragma once
#include "Particle.h"
#include <list>

class ParticleSystem
{
private:
	int scaleX;
	int scaleY;
	int scaleZ;

	int maxParticles;

	float minParticleSize;
	float maxParticleSize;

	float minParticleLifetime;
	float maxParticleLifetime;

	float floatingspeed = 0.05;

	bool gravity;
	bool initialized;

	GLuint* particleTexture;

	std::list<Particle*> particles;

public:
	explicit ParticleSystem(int scaleX, int scaleY, int scaleZ, int maxParticles, float minParticleSize, float maxParticleSize, float maxParticleLifetime, float minParticleLifetime, float floatingspeed, GLuint* tex);
	void init(bool gravity);
	void update();
};
