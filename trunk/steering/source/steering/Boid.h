// Vector3.h: interface for the Vector3 class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <math.h>
#include <stdio.h>
#include "Steering.h"
#include "../base/DisplayListElements.h"
#include <GL/glut.h>
#include <vector>

//OBS: por questoes de simplicidade, todos os atributos sao publicos. 
class Boid 
{

public:
	Vec3 pos,  //posicao
	   dir,  //direcao
	//   lado, //vetor perperndicular a direcao
	   vel;  //velocidade

	Vec3 steeringForce;


	float   raio;
	double  massa;        //massa do actor
	float   timeElapsed;
	double  maxVel;       //velocidade maxima
	double  maxForca;     //forca maxima que o actor pode produzir

	Steering *steering;

	Boid();

	void update(double time_elapsed, Vec3 targetPos, int steeringType);

	void render(DisplayListElements::DL_MODE theMode, int theResolucao);
	void render();

	float radius(void) const ;
	Vec3 position(void) const;
	Vec3 forward() const;

	// groups of (pointers to) abstract vehicles, and iterators over them
	typedef std::vector<Boid*> group;
	typedef group::const_iterator iterator;  

	// more convenient short names for AbstractVehicle group and iterator
	typedef Boid::group BoidGroup;
	typedef Boid::iterator BoidIterator;

	// ----------------------------------------------------------------------------
	// basic flocking
	//
	Vec3 steerToFlock (void);

	// ----------------------------------------------------------------------------
	// used by boid behaviors: is a given vehicle within this boid's neighborhood?
	//
	bool inBoidNeighborhood (const Boid& otherVehicle,
		const float minDistance,
		const float maxDistance,
		const float cosMaxAngle);


	// ----------------------------------------------------------------------------
	// Separation behavior: steer away from neighbors
	//
	Vec3 steerForSeparation (const float maxDistance,
		const float cosMaxAngle,
		const BoidGroup& flock);


	// ----------------------------------------------------------------------------
	// Alignment behavior: steer to head in same direction as neighbors
	//

	Vec3 steerForAlignment (const float maxDistance,
		const float cosMaxAngle,
		const BoidGroup& flock);

	// ----------------------------------------------------------------------------
	// Cohesion behavior: to to move toward center of neighbors
	//

	Vec3 steerForCohesion (const float maxDistance,
		const float cosMaxAngle,
		const BoidGroup& flock);

private:
	// allocate one and share amoung instances just to save memory usage
	// (change to per-instance allocation to be more MP-safe)
	Boid::BoidGroup neighbors;
};

#endif
