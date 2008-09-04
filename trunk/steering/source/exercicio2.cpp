#include <windows.h>		// Header File For Windows
#include <math.h>			// Header File For Windows Math Library
#include <stdio.h>			// Header File For Standard Input/Output
#include <time.h>
#include <stdarg.h>
#include <GL/gl.h>
#include <GL/glut.h>


#include "base/Frames.h"
#include "base/GLFont.h"

GLFont	* font;
Frames  * frames;
float     fps = 100; 
clock_t t1, t2;  

int posicaoluz = 0;
void mudaModoDisplayList(void);
#define X .525731112119133606
#define Z .850650808352039932
#define RESOLUCOES 7
static GLfloat vdata[12][3] = {
	{-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
	{0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
	{Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};
static GLuint tindices[20][3] = {
	{0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
	{8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
	{7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
	{6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
};

int ligacor = 0;
int resolucao = 0;
GLuint dLEsfera[RESOLUCOES];

void init(void)
{

	/* Cria as matrizes responsáveis pelo
	controle de luzes na cena */

	GLfloat ambiente[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat difusa[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat especular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat posicao[] = { 0.0, 3.0, 2.0, 0.0 };
	GLfloat lmodelo_ambiente[] = { 0.2, 0.2, 0.2, 1.0 };


	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	/* Cria e configura a Luz para a cena */

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa);
	glLightfv(GL_LIGHT0, GL_POSITION, posicao);
	glLightfv(GL_LIGHT0, GL_SPECULAR, especular);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodelo_ambiente);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	mudaModoDisplayList();

	frames  = new Frames();
	font    = new GLFont();
	t1 = clock();
}

void error(char * str){
	printf("%s\n", str);
}

void normalize(float v[3]) {
	GLfloat d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if (d == 0.0) {
		error("zero length vector");
		return;
	}
	v[0] /= d; v[1] /= d; v[2] /= d;
}
void normcrossprod(float v1[3], float v2[3], float out[3])
{
	GLint i, j;
	GLfloat length;
	out[0] = v1[1]*v2[2] - v1[2]*v2[1];
	out[1] = v1[2]*v2[0] - v1[0]*v2[2];
	out[2] = v1[0]*v2[1] - v1[1]*v2[0];
	normalize(out);
}

void drawtriangle(float *v1, float *v2, float *v3)
{
		glBegin(GL_TRIANGLES);
    //glBegin(GL_LINE_LOOP);
	glNormal3fv(v1); glVertex3fv(v1);
	glNormal3fv(v2); glVertex3fv(v2);
	glNormal3fv(v3); glVertex3fv(v3);
	glEnd();
}
void subdivide(float *v1, float *v2, float *v3)
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

void subdivide2(float *v1, float *v2, float *v3, long depth)
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

/*
Função responsável pelo desenho das esferas.
Nesta função também serão aplicadas as tranformações
necessárias para o efeito desejado.
*/

long rotaciona=0l;
void display(void)
{
	int i, j;
	float frame_time;

	t2 = clock();
	frame_time  = (double)(t2 - t1) / CLOCKS_PER_SEC;
	t1 = t2;

	fps = frames->getFrames();

	/* Variáveis para definição da capacidade de brilho do material */
	GLfloat especular[] = { 1.0, 1.0, 1.0, 1.0 };

	/* Posição da luz */
	GLfloat posicao[] = { 0.0, 3.0, 2.0, 0.0 };

	/*
	Limpa o buffer de pixels e
	determina a cor padrão dos objetos.
	*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);

	/* Armazena o estado anterior para
	rotação da posição da luz */

	glPushMatrix () ;

	glRotated ((GLdouble) posicaoluz, 1.0, 0.0, 0.0);
	glLightfv (GL_LIGHT0, GL_POSITION, posicao);

	glPopMatrix(); // Posição da Luz
	rotaciona+= 90* frame_time;
	for(int i=0; i<1000; i++) {
		glPushMatrix();

		glColor3f ((i*12347%255)/255.0, (i*6541243%255)/255.0,(i*9899543%255)/255.0);
		glRotatef(rotaciona, 1.0, 1.0, 1.0);
		glTranslatef((i*12347%250)/50.0, (i*6541243%250)/50.0,(i*9899543%250)/50.0);
		//glTranslatef(i%8-4, i%9-4.5, i%10-5);
		glScalef(0.3f,0.3f,0.3f);
		glCallList(dLEsfera[resolucao]);

		glPopMatrix();
	}

	printf("                 \r", fps);
	printf("FPS: %.0f\r", fps);
	//static char text[50];
	//font->startText();
	//glColor3f(1,1,1);
	//
	//sprintf(text, "FPS:  %.0f", fps);
	//font->print(20, 460, text);
	//font->endText(); 


	// Executa os comandos
	glutSwapBuffers();

	glutPostRedisplay();

}

void mudaModoDisplayList(void) 
{
	GLuint tempIndice = glGenLists(RESOLUCOES);

	GLfloat semespecular[4]={0.0,0.0,0.0,1.0};
	
	
	for(int j=0; j<RESOLUCOES; j++) {
		
		dLEsfera[j] = tempIndice+j;
		glNewList(tempIndice+j, GL_COMPILE);
		//comandos de desenho do objeto
		//vértices, transformações, etc.
		/* Define a propriedade do material */
		//refletância do material
		glMaterialfv(GL_FRONT,GL_SPECULAR, semespecular);
		// Define a concentração do brilho
		glMateriali(GL_FRONT,GL_SHININESS,100);

		for (int i = 0; i < 20; i++) {
			subdivide2(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0], j);
		}
		glEndList();
	}
}

/*
Função responsável pelo desenho da tela
Nesta função são determinados o tipo de Projeção
o modelo de Matrizes e
a posição da câmera
Quando a tela é redimensionada os valores
da visão perspectiva são recalculados com base no novo tamanho da tela
assim como o Viewport
*/   

void reshape (int w, int h)
{

	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluLookAt (0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

/* Função responsável pelo controle de teclado
quando pressionada a tecla d, será executada uma rotação no
próprio eixo da esfera menor. Quando pressionada a tecla y
a esfera menor irá rotacionar em torno da esfera maior, em uma
órbida determinada na translação na função display()
A tecla w é responsável por determinar se as esferas serão sólidas
ou aramadas (wire)
*/   

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			resolucao = (key-'0'<RESOLUCOES?key-'0':RESOLUCOES-1);
			
			glutPostRedisplay();
			break;
		case 27:
			exit(0);
			break;
		default:
			break;
	}
}


void mouse(int button, int state, int x, int y)
{
	switch (button) {
	  case GLUT_LEFT_BUTTON:
		  if (state == GLUT_DOWN)
			  glutIdleFunc(NULL);
		  break;
	  case GLUT_RIGHT_BUTTON:
		  posicaoluz = (posicaoluz + 1) % 360;
		  glutPostRedisplay();	  
		  break;
	  default:
		  break;
	}
}

/*
Função principal do programa.
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (500, 500);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Exemplo 2");
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
