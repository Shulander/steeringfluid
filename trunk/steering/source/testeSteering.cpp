#include <windows.h>		// Header File For Windows
#include <math.h>			// Header File For Windows Math Library
#include <stdio.h>			// Header File For Standard Input/Output
#include <time.h>
#include <stdarg.h>
#include <GL/gl.h>
#include <GL/glut.h>


#include "base/Frames.h"
#include "base/GLFont.h"
#include "base/DisplayListElements.h"
#include "steering/Actor.h"

GLFont	* font;
Frames  * frames;
double     fps = 100; 
clock_t t1, t2;  

int posicaoluz = 0;
int ligacor = 0;

GLuint linhasCartesianas;

Actor * atorArrive;
Actor * actorWander;
Actor * actorFlee;
Actor * actorSeek;

int resolucao = 0;
#define RESOLUCOES 7
GLuint dLEsfera[RESOLUCOES];

void defineLinhasCartesianas(void) 
{

	linhasCartesianas = glGenLists(1);
	glNewList(linhasCartesianas, GL_COMPILE);
	//comandos de desenho do objeto
	//vértices, transformações, etc.
	glBegin (GL_LINE_STRIP);
	glColor3f (1.0, 1.0, 1.0);
	glVertex3f (-200.0, 0.0, 0.0);
	glColor3f (1.0, 0.0, 0.0);
	glVertex3f (0.0, 0.0, 0.0);
	glColor3f (1.0, 1.0, 1.0);
	glVertex3f (200.0, 0.0, 0.0);
	glEnd ();
	glBegin (GL_LINE_STRIP);
	glColor3f (1.0, 1.0, 1.0);
	glVertex3f ( 0.0, -200.0,0.0);
	glColor3f (0.0, 1.0, 0.0);
	glVertex3f (0.0, 0.0, 0.0);
	glColor3f (1.0, 1.0, 1.0);
	glVertex3f ( 0.0, 200.0,0.0);
	glEnd ();
	glBegin (GL_LINE_STRIP);
	glColor3f (1.0, 1.0, 1.0);
	glVertex3f ( 0.0,0.0, -200.0);
	glColor3f (0.0, 0.0, 1.0);
	glVertex3f (0.0, 0.0, 0.0);
	glColor3f (1.0, 1.0, 1.0);
	glVertex3f ( 0.0,0.0, 200.0);
	glEnd ();
	glEndList();

	DisplayListElements::criaEsferasDisplayList(dLEsfera, RESOLUCOES);
}

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

	frames  = new Frames();
	font    = new GLFont();
	t1 = clock();

	atorArrive = new Actor();
//	atorArrive->pos = Vec3::RandomUnitVector()*30.0;
	atorArrive->massa = 3;

	actorWander = new Actor();
	actorWander->pos = Vec3::RandomUnitVector()*30.0;
	actorFlee = new Actor();
	actorFlee->pos = Vec3::RandomUnitVector()*30.0;
	actorSeek = new Actor();
	actorSeek->pos = Vec3::RandomUnitVector()*30.0;

	defineLinhasCartesianas();
}

void error(char * str){
	printf("%s\n", str);
}

/*
Função responsável pelo desenho das esferas.
Nesta função também serão aplicadas as tranformações
necessárias para o efeito desejado.
*/

long rotaciona=0l;
void display(void)
{
	float frame_time;

	glLoadIdentity();

	gluLookAt(atorArrive->pos.x, atorArrive->pos.y, atorArrive->pos.z, atorArrive->pos.x+atorArrive->dir.x, atorArrive->pos.y+atorArrive->dir.y, atorArrive->pos.z+atorArrive->dir.z, 0.0, 1.0, 0.0);

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


	//glRotated ((GLdouble) posicaoluz, 1.0, 0.0, 0.0);
	//glLightfv (GL_LIGHT0, GL_POSITION, posicao);

	/* Armazena o estado anterior para
	rotação da posição da luz */


	// desenha linhas guias
	glPushMatrix();
	glCallList(linhasCartesianas);
	glPopMatrix();

	actorWander->update(frame_time, Vec3(0,0,0), WANDER);
	atorArrive->update(frame_time, actorWander->pos, ARRIVE);
	actorFlee->update(frame_time, actorWander->pos, FLEE);
	actorSeek->update(frame_time, actorWander->pos, SEEK);

	atorArrive->pos=atorArrive->pos.sphericalWrapAround(Vec3(0,0,0), 100);
	actorFlee->pos=actorFlee->pos.sphericalWrapAround(Vec3(0,0,0), 100);
	actorSeek->pos=actorSeek->pos.sphericalWrapAround(Vec3(0,0,0), 100);
	actorWander->pos=actorWander->pos.sphericalWrapAround(Vec3(0,0,0), 100);


	glPushMatrix();
	glColor3f (1.0, 1.0, 1.0);
	glScalef(100, 100, 100);
	glCallList(dLEsfera[resolucao]);
	glPopMatrix();
	// esfera de contençao dele
	glColor3f (1.0, 1.0, 1.0);
	atorArrive->render();
	actorFlee->render();
	actorSeek->render();
	actorWander->render();

	// Executa os comandos
	glutSwapBuffers();

	glutPostRedisplay();

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
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 10.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluLookAt (100.0, 100.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

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
	glutInitWindowSize (1024, 768);
	glutInitWindowPosition (100, 0);
	glutCreateWindow ("Teste Steering");
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
