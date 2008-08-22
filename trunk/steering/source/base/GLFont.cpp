#include <stdio.h>
#include <stdarg.h>
#include "GLFont.h"

void GLFont::startText()
{
   glPushAttrib(GL_ENABLE_BIT);
   
   glDisable(GL_COLOR_MATERIAL);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_FOG);
   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_BLEND);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0, 500, 500, 0);

   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
}

void GLFont::print(int x, int y, char *string)
{
   char *c;
   glColor3f(0.8,0,0);

	for (c=string; *c != '\0'; c++) 
    {
		glRasterPos2f(x,y);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		x = x + glutBitmapWidth(GLUT_BITMAP_HELVETICA_18,*c) + 6;
	}/**/
}

void GLFont::endText()
{
   glPopMatrix();

   glPopAttrib();
}
