
#include "ParticleSystem.h"
#include "Coordinate.h"

ParticleSystem::ParticleSystem(int scaleX, int scaleY, int scaleZ, int maxParticles, float minParticleSize, float maxParticleSize, float maxParticleLifetime, float minParticleLifetime, float floatingspeed, GLuint* tex):
	scaleX(scaleX), scaleY(scaleY), scaleZ(scaleZ), maxParticles(maxParticles), minParticleSize(minParticleSize), maxParticleSize(maxParticleSize),
	 minParticleLifetime(minParticleLifetime), maxParticleLifetime(maxParticleLifetime), floatingspeed(floatingspeed), gravity(false), initialized(false), particleTexture(tex)
{
}

void ParticleSystem::init(bool gravity, bool transparent)
{
	initialized = true;
	this->gravity = gravity;
	this->transparent = transparent;
	particles = std::list<Particle*>();
	particlesTransparencyHelper = std::map<float, Particle*>();

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
			}
			else
			{
				if(!transparent)
				{
					(*particle_iterator)->update(); //draws current particle
				}
				else
				{
					const coordinate model((*particle_iterator)->posX, (*particle_iterator)->posY, (*particle_iterator)->posZ);

					const coordinate distance(model.x - 0, model.y - 0, model.z - 0);

					const auto distance_len = sqrt((distance.x *distance.x) + (distance.y *distance.y) + (distance.z *distance.z)); //todo remove sqrt
					particlesTransparencyHelper.insert(std::pair<float, Particle*>(distance_len, (*particle_iterator)));
				}
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
			particle->setGravity(gravity);
			glPopMatrix();
			particles.push_back(particle);
		}
	}
	if(transparent)
	{
		glDepthMask(GL_FALSE);
		//for (auto const& particle : particlesTransparencyHelper) //todo do not fail this is front to back....
		//{
		//	particle.second->update();
		//}
		for (auto particleIterator = particlesTransparencyHelper.rbegin(); particleIterator != particlesTransparencyHelper.rend(); ++particleIterator) {
			particleIterator->second->update();
		}
		glDepthMask(GL_TRUE);
		particlesTransparencyHelper.clear();
	}
}
