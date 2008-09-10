#ifndef __DISPLAYLISTELEMENTS_H__
#define __DISPLAYLISTELEMENTS_H__

#include <stdio.h>
#include <math.h>
#include <GL/glut.h>


class DisplayListElements {

public:
	DisplayListElements (void);

	/************************************************************************/
	//  Função responsável pelo desenho das esferas.
	// Nesta função também serão aplicadas as tranformações
	// necessárias para o efeito desejado. 
	/************************************************************************/

	static void criaEsferasDisplayList(GLuint *theDLEsfera, int theResolucoes);

private:
	static GLfloat vdata[12][3];
	static GLuint tindices[20][3];
	static double X;
	static double Z;

	static void error(char * str);
	static void normalize(float v[3]);
	static void normcrossprod(float v1[3], float v2[3], float out[3]);
	static void drawtriangle(float *v1, float *v2, float *v3);
	static void subdivide(float *v1, float *v2, float *v3);
	static void subdivide2(float *v1, float *v2, float *v3, long depth);

};
#endif