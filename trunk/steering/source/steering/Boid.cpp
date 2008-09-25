#include "Boid.h"
#include "Proximity.h"

Boid::Boid()
{
   massa		= 0.4;
   timeElapsed	= 0;
   maxVel		= 20;
   maxForca		= 2;
   raio			=2;
   
   pos.set(0,0,0);
   vel.set(0,0,0);
   steeringForce.set(0,0,0);

   steering = new Steering();
   Proximity::addNeighbor(this);
}

void Boid::update(double time_elapsed, Vec3 targetPos, int steeringType)
{    
   timeElapsed = time_elapsed;      
   
   steeringForce = steering->calculateSteering(this, targetPos, steeringType);
 
   Vec3 acceleration = steeringForce / massa;

   //update velocity
   vel += (acceleration * time_elapsed); 

   //make sure vehicle does not exceed maximum velocity
   vel=vel.truncateLength(maxVel);

   //update the position
   pos += vel * time_elapsed;

   //update the heading if the vehicle has a non zero velocity
   if (vel.lengthSquared() > 0.00000001)
   {    
      Vec3 tmp(vel);   
      tmp=tmp.normalize();
      dir  = tmp;
//      lado = dir.perp();
   }
}


//**************************************************************************************************   
//**************************************************************************************************   
//**************************************************************************************************   


void Boid::render(DisplayListElements::DL_MODE theMode, int theResolucao) {
	//renderiza o steering behavior
	steering->render(this, theMode, theResolucao);


	//desenha o actor
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glColor3f(0.5f, 0.7f, 1.0f);
	glScalef(3, 3, 3);
	DisplayListElements::desenhaEsferasDisplayList(theMode, theResolucao);
	glPopMatrix();

	glPushMatrix();
	//   glLoadIdentity();

	//desenha o vetor direcao
	glColor3d(0, 1, 0);
	glTranslatef(pos.x, pos.y, pos.z);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(dir.x*30, dir.y*30, dir.z*30);
	glEnd();   

	//desenha o vetor forca
	glColor3f(0,0,1);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(steeringForce.x, steeringForce.y, steeringForce.z);
	glEnd();     

	//calcula o angulo do personagem
	Vec3 up;
	up.cross(dir, steeringForce);
	up=up.normalize();
	Vec3 side;
	side.cross(dir, up);
	side=side.normalize();

	glPopMatrix();
}

void Boid::render()
{
	render(DisplayListElements::WIRED, 1);
}

float Boid::radius() const { return raio; }
Vec3 Boid::position() const { return pos; }
Vec3 Boid::forward() const { return dir; }

// ----------------------------------------------------------------------------
// basic flocking
//
Vec3 Boid::steerToFlock (void)
{
	// avoid obstacles if needed
	// todo: this should probably be moved elsewhere
	//	const Vec3 avoidance = steerToAvoidObstacles (2.0f, obstacles);
	//	if (avoidance != Vec3::zero) return avoidance;

	const float separationRadius =  10.0f;
	const float separationAngle  = -0.707f;
	const float separationWeight =  6.0f;

	const float alignmentRadius = 11.5f;
	const float alignmentAngle  = 0.7f;
	const float alignmentWeight = 3.7f;

	const float cohesionRadius = 12.0f;
	const float cohesionAngle  = -0.15f;
	const float cohesionWeight = 4.8f;

	const float maxRadius = std::max (separationRadius,
		std::max (alignmentRadius,
		cohesionRadius));

	// find all flockmates within maxRadius using proximity database
	neighbors.clear();
	Proximity::findNeighbors (pos, maxRadius, neighbors);

//#ifndef NO_LQ_BIN_STATS
//	// maintain stats on max/min/ave neighbors per boids
//	size_t count = neighbors.size();
//	if (maxNeighbors < count) maxNeighbors = count;
//	if (minNeighbors > count) minNeighbors = count;
//	totalNeighbors += count;
//#endif // NO_LQ_BIN_STATS

	// determine each of the three component behaviors of flocking
	const Vec3 separation = steerForSeparation (separationRadius,
		separationAngle,
		neighbors);
	const Vec3 alignment  = steerForAlignment  (alignmentRadius,
		alignmentAngle,
		neighbors);
	const Vec3 cohesion   = steerForCohesion   (cohesionRadius,
		cohesionAngle,
		neighbors);

	// apply weights to components (save in variables for annotation)
	const Vec3 separationW = separation * separationWeight;
	const Vec3 alignmentW = alignment * alignmentWeight;
	const Vec3 cohesionW = cohesion * cohesionWeight;

	// annotation
	// const float s = 0.1;
	// annotationLine (position, position + (separationW * s), gRed);
	// annotationLine (position, position + (alignmentW  * s), gOrange);
	// annotationLine (position, position + (cohesionW   * s), gYellow);

	return separationW + alignmentW + cohesionW;
}

// ----------------------------------------------------------------------------
// used by boid behaviors: is a given vehicle within this boid's neighborhood?
//
bool Boid::inBoidNeighborhood (const Boid& otherVehicle,
						 const float minDistance,
						 const float maxDistance,
						 const float cosMaxAngle)
{
	if (&otherVehicle == this)
	{
		return false;
	}
	else
	{
		const Vec3 offset = otherVehicle.position() - position();
		const float distanceSquared = offset.lengthSquared ();

		// definitely in neighborhood if inside minDistance sphere
		if (distanceSquared < (minDistance * minDistance))
		{
			return true;
		}
		else
		{
			// definitely not in neighborhood if outside maxDistance sphere
			if (distanceSquared > (maxDistance * maxDistance))
			{
				return false;
			}
			else
			{
				// otherwise, test angular offset from forward axis
				const Vec3 unitOffset = offset / sqrt (distanceSquared);
				const float forwardness = forward().dot (unitOffset);
				return forwardness > cosMaxAngle;
			}
		}
	}
}


// ----------------------------------------------------------------------------
// Separation behavior: steer away from neighbors
//
Vec3 Boid::steerForSeparation (const float maxDistance,
						 const float cosMaxAngle,
						 const BoidGroup& flock)
{
	// steering accumulator and count of neighbors, both initially zero
	Vec3 steering;

	// for each of the other vehicles...
	BoidIterator flockEndIter = flock.end();
	for (BoidIterator otherVehicle = flock.begin(); otherVehicle != flockEndIter; ++otherVehicle )
	{
		// todo ver se necessita do radius.
		if (inBoidNeighborhood (**otherVehicle, radius()*3, maxDistance, cosMaxAngle))
		{
			// add in steering contribution
			// (opposite of the offset direction, divided once by distance
			// to normalize, divided another time to get 1/d falloff)
			const Vec3 offset = (**otherVehicle).position() - position();
			const float distanceSquared = offset.dot(offset);
			steering += (offset / -distanceSquared);
		}
	}

	steering = steering.normalize();

	return steering;
}


// ----------------------------------------------------------------------------
// Alignment behavior: steer to head in same direction as neighbors
//

Vec3 Boid::steerForAlignment (const float maxDistance,
						const float cosMaxAngle,
						const BoidGroup& flock)
{
	// steering accumulator and count of neighbors, both initially zero
	Vec3 steering;
	int neighbors = 0;

	// for each of the other vehicles...
	for (BoidIterator otherVehicle = flock.begin(); otherVehicle != flock.end(); otherVehicle++)
	{
		if (inBoidNeighborhood (**otherVehicle, radius()*3, maxDistance, cosMaxAngle))
		{
			// accumulate sum of neighbor's heading
			steering += (**otherVehicle).forward();

			// count neighbors
			neighbors++;
		}
	}

	// divide by neighbors, subtract off current heading to get error-
	// correcting direction, then normalize to pure direction
	if (neighbors > 0) steering = ((steering / (float)neighbors) - forward()).normalize();

	return steering;
}


// ----------------------------------------------------------------------------
// Cohesion behavior: to to move toward center of neighbors
//

Vec3 Boid::steerForCohesion (const float maxDistance,
					   const float cosMaxAngle,
					   const BoidGroup& flock)
{
	// steering accumulator and count of neighbors, both initially zero
	Vec3 steering;
	int neighbors = 0;

	// for each of the other vehicles...
	for (BoidIterator otherVehicle = flock.begin(); otherVehicle != flock.end(); otherVehicle++)
	{
		if (inBoidNeighborhood (**otherVehicle, radius()*3, maxDistance, cosMaxAngle))
		{
			// accumulate sum of neighbor's positions
			steering += (**otherVehicle).position();

			// count neighbors
			neighbors++;
		}
	}

	// divide by neighbors, subtract off current position to get error-
	// correcting direction, then normalize to pure direction
	if (neighbors > 0) steering = ((steering / (float)neighbors) - position()).normalize();

	return steering;
}