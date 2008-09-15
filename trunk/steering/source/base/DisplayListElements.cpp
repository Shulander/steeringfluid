#include "DisplayListElements.h"

DisplayListElements::DisplayListElements(void) {

}

/************************************************************************/
/* Método responsável por desenhar as esferas utilizando display list   */
/* Caso não esteja definida a mesma é definida e em seguida desenhada   */
/************************************************************************/
void DisplayListElements::desenhaEsferasDisplayList(DisplayListElements::DL_MODE theMode, int theResolucao) 
{
	// mantem as resolucoes a um nível aceitável dentro dos padroes maximos e mínimos
	theResolucao = (theResolucao>=DL_RESOLUCOES?DL_RESOLUCOES-1: theResolucao);
	theResolucao = (theResolucao<0?0:theResolucao);

	if(dLEsfera[theMode][theResolucao]==-1) {
		mMode = theMode;
		criaEsferasDisplayList(dLEsfera[theMode], DL_RESOLUCOES);
	}

	glCallList(dLEsfera[theMode][theResolucao]);
}

/************************************************************************/
/* Método responsável por desenhar as esferas utilizando display list   */
/* Caso não esteja definida a mesma é definida e em seguida desenhada   */
/************************************************************************/
void DisplayListElements::desenhaTeaPot(DisplayListElements::DL_MODE theMode) 
{
	if(dLTeaPot[theMode]==-1) {
		mMode = theMode;
		criaTeaPotDisplayList(&dLTeaPot[theMode]);
	}

	glCallList(dLTeaPot[theMode]);
}

/************************************************************************/
// Função responsável pelo desenho dos teapot.
// Nesta função também serão aplicadas as tranformações
// necessárias para o efeito desejado.
/************************************************************************/

void DisplayListElements::criaTeaPotDisplayList(GLuint *theDLTeaPot) 
{
	GLuint tempIndice = glGenLists(1);

	GLfloat semespecular[4]={0.0,0.0,0.0,1.0};
	theDLTeaPot[0] = tempIndice;
	glNewList(tempIndice, GL_COMPILE);
	//comandos de desenho do objeto
	//vértices, transformações, etc.
	// Define a propriedade do material
	//refletância do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, semespecular);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,100);

	if(mMode==DisplayListElements::SOLID){ glutSolidTeapot(1.0); } 
	else {	glutWireTeapot(1.0); }

	glEndList();
}

/************************************************************************/
// Função responsável pelo desenho das esferas.
// Nesta função também serão aplicadas as tranformações
// necessárias para o efeito desejado.                                                                    */
/************************************************************************/

void DisplayListElements::criaEsferasDisplayList(GLuint *theDLEsfera, int theResolucoes) 
{
	GLuint tempIndice = glGenLists(theResolucoes);

	GLfloat semespecular[4]={0.0,0.0,0.0,1.0};


	for(int j=0; j<theResolucoes; j++) {
		theDLEsfera[j] = tempIndice+j;
		glNewList(tempIndice+j, GL_COMPILE);
		//comandos de desenho do objeto
		//vértices, transformações, etc.
		// Define a propriedade do material
		//refletância do material
		glMaterialfv(GL_FRONT,GL_SPECULAR, semespecular);
		// Define a concentração do brilho
		glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,100);		

		for (int i = 0; i < 20; i++) {
			subdivide2(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0], j);
		}
		glEndList();
	}
}

void DisplayListElements::error(char * str){
	printf("%s\n", str);
}

void DisplayListElements::normalize(float v[3]) {
	GLfloat d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if (d == 0.0) {
		error("zero length vector");
		return;
	}
	v[0] /= d; v[1] /= d; v[2] /= d;
}
void DisplayListElements::normcrossprod(float v1[3], float v2[3], float out[3])
{
	out[0] = v1[1]*v2[2] - v1[2]*v2[1];
	out[1] = v1[2]*v2[0] - v1[0]*v2[2];
	out[2] = v1[0]*v2[1] - v1[1]*v2[0];
	normalize(out);
}

void DisplayListElements::drawtriangle(float *v1, float *v2, float *v3)
{
	if(mMode==DisplayListElements::SOLID){ glBegin(GL_TRIANGLES); } 
	else {	glBegin(GL_LINE_LOOP); }
	glNormal3fv(v1); glVertex3fv(v1);
	glNormal3fv(v2); glVertex3fv(v2);
	glNormal3fv(v3); glVertex3fv(v3);
	glEnd();
}
void DisplayListElements::subdivide(float *v1, float *v2, float *v3)
{
	GLfloat v12[3], v23[3], v31[3];
	GLint i;
	for (i = 0; i < 3; i++) {
		v12[i] = v1[i]+v2[i];
		v23[i] = v2[i]+v3[i];
		v31[i] = v3[i]+v1[i];
	}
	normalize(v12);
	normalize(v23);
	normalize(v31);
	drawtriangle(v1, v12, v31);
	drawtriangle(v2, v23, v12);
	drawtriangle(v3, v31, v23);
	drawtriangle(v12, v23, v31);
}

void DisplayListElements::subdivide2(float *v1, float *v2, float *v3, long depth)
{
	GLfloat v12[3], v23[3], v31[3];
	GLint i;
	if (depth == 0) {
		drawtriangle(v1, v2, v3);
		return;
	}
	for (i = 0; i < 3; i++) {
		v12[i] = v1[i]+v2[i];
		v23[i] = v2[i]+v3[i];
		v31[i] = v3[i]+v1[i];
	}
	normalize(v12);
	normalize(v23);
	normalize(v31);
	subdivide2(v1, v12, v31, depth-1);
	subdivide2(v2, v23, v12, depth-1);
	subdivide2(v3, v31, v23, depth-1);
	subdivide2(v12, v23, v31, depth-1);
}

GLfloat DisplayListElements::vdata[12][3] = {
	{-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
	{0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
	{Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};
GLuint DisplayListElements::tindices[20][3] = {
	{0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
	{8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
	{7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
	{6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
};
double DisplayListElements::X = .525731112119133606;
double DisplayListElements::Z = .850650808352039932;
GLuint DisplayListElements::dLEsfera[2][DL_RESOLUCOES] = {{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1}};
GLuint DisplayListElements::dLTeaPot[2] = {-1,-1};
DisplayListElements::DL_MODE DisplayListElements::mMode = DisplayListElements::WIRED;