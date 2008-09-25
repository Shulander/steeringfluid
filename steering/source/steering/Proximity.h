#ifndef PROXIMITY_H
#define PROXIMITY_H


#include <algorithm>
#include <vector>
#include "../base/Vec3.hpp"
#include "Boid.h"

class Proximity
{
public:
	static void findNeighbors (const Vec3& center, const float radius, Boid::group& results);
	static void addNeighbor (Boid * boid);
	static Proximity *instance();
protected:
private:
	static Proximity *singletonInstance;
	static Boid::group boids;
};



#endif // PROXIMITY_H
