#include "Proximity.h"

void Proximity::findNeighbors (const Vec3& center, const float radius, Boid::group& results)
{
	// loop over all tokens
	const float r2 = radius * radius;
	for (Boid::BoidIterator i = boids.begin(); i != boids.end(); i++) {
		const Vec3 offset = center - (**i).position();
		const float d2 = offset.lengthSquared();

		// push onto result vector when within given radius
		if (d2 < r2) results.push_back ((*i));
	}
}

void Proximity::addNeighbor (Boid * boid)
{	
	boids.push_back(boid);
}

Proximity * Proximity::instance()
{  
	// is it the first call?
	if (singletonInstance == NULL)	{  
		singletonInstance = new Proximity();
	}
	return singletonInstance;
}

Proximity *Proximity::singletonInstance=NULL;
Boid::group Proximity::boids;