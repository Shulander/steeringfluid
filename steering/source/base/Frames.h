#ifndef _FRAMES_H___
#define _FRAMES_H___	

#include <stdio.h>
#include <time.h>

#define FRAMES 10 

////////////////////////////////////////////////////////////////////////////////////////
class Frames{
   clock_t t1, t2;   
   long    cont_frames;
   float   fps, fps_old;
public:
   Frames()
   {
      t1 = clock();
	  cont_frames = 0;
      fps_old = 80; //valor inicial
   }

   float getFrames()
   {
      double tempo;

      t2 = clock();
      tempo  = (double)(t2 - t1) / CLOCKS_PER_SEC;
      cont_frames++;
      //apos FRAMES frames, calcula o fps medio.
      if(cont_frames==FRAMES)
      {
         t1 = t2;
         fps = FRAMES/tempo;
         cont_frames=0;
         fps_old = fps; 

         //printf("\r %0.2lf frames per second  ", fps);
         return fps;
      }
      //se nao passaram 6 frames, retorna a taxa antiga
      return fps_old;
   }
};

#endif
