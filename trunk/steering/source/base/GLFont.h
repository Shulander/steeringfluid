// GLFont.h: interface for the GLFont class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _GLFONT_H__
#define _GLFONT_H__
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>

class GLFont  
{
public:
	void print(int x, int y, char *string);
	void endText();
	void startText();
	GLFont(){}
protected:
	GLboolean color_material, texture_2d, fog, lighting, depth_test;
};

#endif 
