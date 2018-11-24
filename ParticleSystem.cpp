
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int scaleX, int scaleY, int scaleZ, int maxParticles, float minParticleSize, float maxParticleSize, float maxParticleLifetime, float minParticleLifetime, float floatingspeed, GLuint* tex):
	scaleX(scaleX), scaleY(scaleY), scaleZ(scaleZ), maxParticles(maxParticles), minParticleSize(minParticleSize), maxParticleSize(maxParticleSize),
	 minParticleLifetime(minParticleLifetime), maxParticleLifetime(maxParticleLifetime), floatingspeed(floatingspeed), gravity(false), initialized(false), particleTexture(tex)
{
}

void ParticleSystem::init(bool gravity)
{
	initialized = true;
	this->gravity = gravity;
	particles = std::list<Particle*>();
}

void ParticleSystem::update()
{
	if(initialized)
	{
		auto particle_iterator = particles.begin();
		while (particle_iterator != particles.end())
		{
			if (!(*particle_iterator)->isAlive())
			{
				delete (*particle_iterator);
				particles.erase(particle_iterator++);  // alternatively, particleIterator = items.erase(particleIterator);
				//actually delete particle
			}
			else
			{
				(*particle_iterator)->update();
				++particle_iterator;
			}
		}
		if(particles.size() < maxParticles)
		{
			glPushMatrix();
			const float pos_x = rand() % (-1 * scaleX - 1 * scaleX + 1) + 1 * scaleX;
			const float pos_y = rand() % (-1 * scaleY - 1 * scaleY + 1) + 1 * scaleY;
			const float pos_z = rand() % (-1 * scaleZ - 1 * scaleZ + 1) + 1 * scaleZ;
			const auto particle = new Particle(pos_x, pos_y, pos_z, minParticleSize, maxParticleSize, minParticleLifetime, maxParticleLifetime, floatingspeed, particleTexture);
			glPopMatrix();
			particles.push_back(particle);
		}
	}
}
