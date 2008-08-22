
#ifndef __STEERING_H
#define __STEERING_H

#define ARRIVE  0
#define SEEK    1
#define FLEE    2
#define WANDER  3
#define PURSUIT 4

#include "Vec3.h"
#include "utils.h"

class Actor;

class Steering
{
   //usado no comportametno Wander
   Vec3     wanderTarget; 
   float       wanderJitter;
   float       wanderRadius;
   float       wanderDistance;
   
   //usado no comportamento WallAvoidance
   Vec3 antenas[3];
   float   wallDetectionFeelerLength; //comprimento das antenas
   
   //definicao de um muro: 0 e 1 definem o ponto e 2 o vetor normal do muro
   Vec3 wall[3];
   
   int steeringType;

public:
   Steering();

   Vec3 calculateSteering(Actor *actor, Vec3 targetPos, int steeringType);
   
   void render(Actor *actor); 

private:
   Vec3 seek(Actor *actor, Vec3 targetPos);
   
   Vec3 flee(Actor *actor, Vec3 targetPos);
   
   Vec3 arrive(Actor *actor, Vec3 targetPos);

   Vec3 wander(Actor *actor);

   Vec3 wallAvoidance(Actor *actor);
   
   
   //********************************
   float randomClamped();
   void drawCircle(float raio);
   void createFeelers(Actor *actor);
};


#endif
