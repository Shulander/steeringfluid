#ifndef __DISPLAYLISTELEMENTS_H__
#define __DISPLAYLISTELEMENTS_H__

#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#define DL_RESOLUCOES 5

class DisplayListElements {

public:

	static enum DL_MODE{
		SOLID=0,
		WIRED=1
	};

	DisplayListElements (void);

	static void desenhaEsferasDisplayList(DL_MODE theMode, int theResolucao);
	static void desenhaTeaPot(DL_MODE theMode);

private:
	static GLfloat vdata[12][3];
	static GLuint tindices[20][3];
	static double X;
	static double Z;
	static DL_MODE mMode;
	static GLuint dLEsfera[2][DL_RESOLUCOES];
	static GLuint dLTeaPot[2];

	static void error(char * str);
	static void normalize(float v[3]);
	static void normcrossprod(float v1[3], float v2[3], float out[3]);
	static void drawtriangle(float *v1, float *v2, float *v3);
	static void subdivide(float *v1, float *v2, float *v3);
	static void subdivide2(float *v1, float *v2, float *v3, long depth);


	/************************************************************************/
	//  Função responsável pelo desenho das esferas.
	// Nesta função também serão aplicadas as tranformações
	// necessárias para o efeito desejado. 
	/************************************************************************/
	static void criaEsferasDisplayList(GLuint *theDLEsfera, int theResolucoes);

	/************************************************************************/
	// Função responsável pelo desenho dos teapot.
	// Nesta função também serão aplicadas as tranformações
	// necessárias para o efeito desejado.                                  
	/************************************************************************/

	static void DisplayListElements::criaTeaPotDisplayList(GLuint *theDLTeaPot);

};
#endif