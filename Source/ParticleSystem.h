#pragma once
#include "Particle.h"
#include <list>
#include <map>

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
	bool transparent;
	bool initialized;
	bool discomode;

	GLuint* particleTexture;

	std::list<Particle*> particles;

	std::map<float, Particle*> particlesTransparencyHelper;


public:
	explicit ParticleSystem(int scaleX, int scaleY, int scaleZ, int maxParticles, float minParticleSize, float maxParticleSize, float maxParticleLifetime, float minParticleLifetime, float floatingspeed, GLuint* tex);
	void init(bool gravity, bool transparent);
	void update();
	void setGravity(bool gravity)
	{
		this->gravity = gravity;
		for (Particle* particle : particles)
		{
			particle->setGravity(gravity);
		}
	}
	bool getGravity()
	{
		return this->gravity;
	}
	bool getDiscomode()
	{
		return this->discomode;
	}
	void setDiscomode(bool discomode)
	{
		this->discomode = discomode;
		for (Particle* particle : particles)
		{
			particle->setDiscomode(discomode);
		}
	}
};
