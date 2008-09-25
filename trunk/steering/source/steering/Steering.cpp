#include "Boid.h"
#include "Steering.h"
#include <math.h>


Steering::Steering()
{
   wanderJitter   = 10.0; // nervosismo ;)
   wanderRadius   = 10.0;
   wanderDistance = 13.0;

   wallDetectionFeelerLength = 100;
   
   //create a vector to a target position on the wander circle
   double theta = -0.5; //valor aleatorio
   wanderTarget.set(wanderRadius * cos(theta),  wanderRadius * sin(theta),  wanderRadius * sin(theta)); 
   
   //define o muro
   wall[0].set(200,200,200);
   wall[1].set(300,300,300);
   Vec3 t;
   t.cross(wall[0],wall[1]);
   t=t.normalize();
   wall[2] = (t); //vetor perpendicular

//   quadratic=gluNewQuadric();
}
   
Vec3 Steering::calculateSteering(Boid *actor, Vec3 targetPos, int type)
{
   Vec3 wallForce = wallAvoidance(actor);
   if( wallForce.lengthSquared() > 1 )
   {
      return wallForce;
   }

   steeringType = type;
   switch(steeringType)
   {
       case SEEK: 
           return seek(actor, targetPos);
           break;

       case FLEE: 
           return flee(actor, targetPos);
           break;

       case ARRIVE: 
           return arrive(actor, targetPos);
           break;

       case WANDER: 
           return wander(actor);
           break;

	   default:
		   return Vec3::RandomUnitVector();
		   break;
   }
}

Vec3 Steering::seek(Boid *actor, Vec3 targetPos)
{
   Vec3 desiredVelocity;
   desiredVelocity = targetPos - actor->pos;
   desiredVelocity=desiredVelocity.normalize();
   desiredVelocity *= actor->maxVel;

   return (desiredVelocity - actor->vel);
}
   

Vec3 Steering::flee(Boid *actor, Vec3 targetPos)
{
   double panicDistance = 100;

   if( Vec3::distance(actor->pos, targetPos)/*actor->pos.distance(targetPos)*/ > panicDistance )
   {
     return Vec3(0,0,0);
   }
   Vec3 desiredVelocity;
   desiredVelocity = actor->pos - targetPos;
   desiredVelocity=desiredVelocity.normalize();
   desiredVelocity *= actor->maxVel;

   return (desiredVelocity - actor->vel);
}

Vec3 Steering::arrive(Boid *actor, Vec3 targetPos)
{
   Vec3 toTarget = targetPos - actor->pos;
    
   //calculate the distance to the target
   double dist = toTarget.lengthSquared();
   float deceleration = 1;
    
   if(dist > 0)
   {
      //calculate the speed required to reach the target given the desired
      //deceleration
      double speed = dist / deceleration;     
        
      //make sure the velocity does not exceed the max
      speed = speed < actor->maxVel ? speed : actor->maxVel;
        
      //from here proceed just like Seek except we don't need to normalize 
      //the ToTarget vector because we have already gone to the trouble
      //of calculating its length: dist. 
      Vec3 desiredVelocity = toTarget * speed / dist;
        
      return (desiredVelocity - actor->vel);
   }
   return Vec3(0,0,0);
}


//--------------------------- Wander -------------------------------------
//  This behavior makes the agent wander about randomly
//------------------------------------------------------------------------
Vec3 Steering::wander(Boid *actor)
{ 
   //this behavior is dependent on the update rate, so this line must
   //be included when using time independent framerate.
   double JitterThisTimeSlice = wanderJitter * actor->timeElapsed * 9;

   Vec3 tmp(randomClamped() * JitterThisTimeSlice,
	   randomClamped() * JitterThisTimeSlice,
	   randomClamped() * JitterThisTimeSlice);
   
   //printf("\n\n J= %lf", JitterThisTimeSlice);
   //wanderTarget.print();
   //tmp.print();
   
   //first, add a small random vector to the target's position
   wanderTarget += tmp;

   //reproject this new vector back on to a unit circle
   wanderTarget=wanderTarget.normalize();

   //increase the length of the vector to the same as the radius
   //of the wander circle
   wanderTarget *= wanderRadius;
//   wanderTarget.print();

   //Pozzer
   Vec3 target(actor->pos);
   target += (actor->dir*wanderDistance);
   target += wanderTarget;
   
   return target - actor->pos; 
} 


Vec3 Steering::wallAvoidance(Boid *actor)
{
//   createFeelers(actor);

   double distance = 0.0;

   Vec3 SteeringForce,
           point;         //used for storing temporary info

   //examine each feeler in turn
   for (int i=0; i<3; i++)
   {
     // if (LineIntersection2D(actor->pos, antenas[i], wall[0], wall[1], distance, point))
     // {
     //    //calculate by what distance the projected position of the agent
     //    //will overshoot the wall
     //    Vec3 OverShoot = antenas[i] - point;

     //    //create a force in the direction of the wall normal, with a 
     //    //magnitude of the overshoot
     //    //normal[2] eh a normal da parede definida por [ wall[0], wall[1] ]
     //    SteeringForce = wall[2] * OverShoot.lengthSquared();
     //    
     //    //determina a direcao da forca em funcao da posicao relativa do muro e do actor
     //    Vec3 v1(wall[0] - wall[1]);
     //    Vec3 v2(wall[0] - actor->pos);
     //    int side = v1.side(v2);
     //    
     //    //printf("\nColidiu lado %d antena %d ", side, i);
     //    if( side == -1 )
     //       return SteeringForce;
     //    else
     //       return SteeringForce.reverse();
     //}
  }//next feeler

  return SteeringForce;

}


void Steering::render(Boid *actor, DisplayListElements::DL_MODE theMode, int theResolucao)
{
	Vec3 target(actor->pos);
	target += (actor->dir*wanderDistance);

	Vec3 wTarget(target);
	wTarget+=wanderTarget;

	if( steeringType == WANDER )
	{
		//desenha o circulo na frente do actor
		glPushMatrix();
		glColor3f(1,0,0);
		glTranslated(target.x, target.y, target.z);
		glScalef(wanderRadius, wanderRadius, wanderRadius);
		DisplayListElements::desenhaEsferasDisplayList(theMode, theResolucao);
		glPopMatrix();

		//desenha o target
		glPushMatrix();
		glTranslated(wTarget.x, wTarget.y, wTarget.z);
		glColor3f(1,0.6,0);
		glScalef(2, 2, 2);
		DisplayListElements::desenhaEsferasDisplayList(theMode, theResolucao);
		glPopMatrix();
	}

	//desenha o muro
	//glColor3f(0,1,0);
	//glPushMatrix();
	//glLoadIdentity();
	//glBegin(GL_LINES);
	//   glVertex3d(wall[0].x, wall[0].y, wall[0].z);
	//   glVertex3d(wall[1].x, wall[1].y, wall[1].z);
	//glEnd();     

	//desenha as antenas
	//glColor3f(1,0,0);
	//glPushMatrix();
	//glLoadIdentity();
	//glTranslatef(actor->pos.x, actor->pos.y, actor->pos.z);
	//glBegin(GL_LINES);
	//   for(int i=0; i<3; i++)
	//   {
	//      Vec3 tmp(antenas[i]);
	//      tmp -= actor->pos;
	//      glVertex3d(0, 0, 0);
	//      glVertex3d(tmp.x, tmp.y, tmp.z);
	//   }
	//glEnd();  
	//glPopMatrix();
	//glPopMatrix();

}
//*************************************************************
void Steering::render(Boid *actor)
{
	render(actor, DisplayListElements::WIRED, 1);
}

void Steering::drawSphere(float raio)
{
	glutWireSphere(raio, 10, 10);
//   gluSphere(quadratic,raio,8,8);
}

//retorna valor entre [-1, 1]
float Steering::randomClamped()
{
   float r = 1.0 - ((rand()%1000)/500.0);
   return r;
}
