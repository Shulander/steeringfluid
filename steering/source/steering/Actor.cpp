#include "Actor.h"

Actor::Actor()
{
   massa       = 0.4;
   timeElapsed = 0;
   maxVel      = 150;
   maxForca    = 2;
   
   pos.set(200,200,200);
   vel.set(0,0,0);
   steeringForce.set(0,0,0);

   steering = new Steering();
}

void Actor::update(double time_elapsed, Vec3 targetPos, int steeringType)
{    
   timeElapsed = time_elapsed;      
   
   steeringForce = steering->calculateSteering(this, targetPos, steeringType);
 
   Vec3 acceleration = steeringForce / massa;

   //update velocity
   vel += (acceleration * time_elapsed); 

   //make sure vehicle does not exceed maximum velocity
   vel.truncateLength(maxVel);

   //update the position
   pos += vel * time_elapsed;

   //update the heading if the vehicle has a non zero velocity
   if (vel.lengthSquared() > 0.00000001)
   {    
      Vec3 tmp(vel);   
      tmp.normalize();
      dir  = tmp;
//      lado = dir.perp();
   }
}


//**************************************************************************************************   
//**************************************************************************************************   
//**************************************************************************************************   


void Actor::render()
{
   //renderiza o steering behavior
   steering->render(this);

   glPushMatrix();
   glLoadIdentity();

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
   //Vec3 vet(0,1,0);
   //float ang = dir.angle(vet);
   //ang *= (180/3.14);


   //calcula o angulo do personagem
   Vec3 up;
   up.cross(dir, steeringForce);
   up.normalize();
   Vec3 side;
   side.cross(dir, up);
   side.normalize();

   //desenha o actor
   glColor3f(0.0f, 1.0f, 0.0f);
//   glRotatef(-ang, 0, 0, 1);
   glBegin(GL_TRIANGLES);
      glVertex3d(-5, -5, 0);
      glVertex3d(5, -5, 0);
      glVertex3d(0, 15, 0);
   glEnd();   

   glPopMatrix();
}
