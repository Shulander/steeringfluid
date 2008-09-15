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

int gResolucao = 0;
DisplayListElements::DL_MODE gMode = DisplayListElements::WIRED;
#define RESOLUCOES 7

void defineLinhasCartesianas(void) 
{
	linhasCartesianas = glGenLists(1);
	glNewList(linhasCartesianas, GL_COMPILE);
	//comandos de desenho do objeto
	//v�rtices, transforma��es, etc.
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
}

void init(void)
{
	/* Cria as matrizes respons�veis pelo
	controle de luzes na cena */

	GLfloat ambiente[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat difusa[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat especular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat posicao[] = { 0.0, 3.0, 2.0, 0.0 };
	GLfloat lmodelo_ambiente[] = { 0.2, 0.2, 0.2, 1.0 };


	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
//	glShadeModel(GL_SMOOTH);
	glShadeModel(GL_FLAT);

	/* Cria e configura a Luz para a cena */

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa);
	glLightfv(GL_LIGHT0, GL_POSITION, posicao);
	glLightfv(GL_LIGHT0, GL_SPECULAR, especular);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodelo_ambiente);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);


	//Armazena o estado anterior para
	//rota��o da posi��o da luz 

	glPushMatrix () ;

	glRotated ((GLdouble) posicaoluz, 1.0, 0.0, 0.0);
	glLightfv (GL_LIGHT0, GL_POSITION, posicao);

	glPopMatrix(); // Posi��o da Luz

	/* Armazena o estado anterior para
	rota��o da posi��o da luz */

	frames  = new Frames();
	font    = new GLFont();
	t1 = clock();

	atorArrive = new Actor();
	atorArrive->pos = Vec3::RandomUnitVector()*30.0;
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
Fun��o respons�vel pelo desenho das esferas.
Nesta fun��o tamb�m ser�o aplicadas as tranforma��es
necess�rias para o efeito desejado.
*/

long rotaciona=0l;
void display(void)
{

	glLoadIdentity();
	gluLookAt(atorArrive->pos.x, atorArrive->pos.y, atorArrive->pos.z, atorArrive->pos.x+atorArrive->dir.x, atorArrive->pos.y+atorArrive->dir.y, atorArrive->pos.z+atorArrive->dir.z, 0.0, 1.0, 0.0);
	//	gluLookAt (100.0, 100.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//Limpa o buffer de pixels e
	//determina a cor padr�o dos objetos.

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);

	float frame_time;

	t2 = clock();
	frame_time  = (double)(t2 - t1) / CLOCKS_PER_SEC;
	t1 = t2;

	fps = frames->getFrames();


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


	// esfera de conten�ao dele
	glPushMatrix();
	glColor3f (1.0, 1.0, 1.0);
	glScalef(100, 100, 100);
	DisplayListElements::desenhaEsferasDisplayList(DisplayListElements::WIRED, gResolucao);
	glPopMatrix();

	glPushMatrix();
	glColor3f (0.0, 1.0, 1.0);
	glScalef(5, 5, 5);
	DisplayListElements::desenhaTeaPot(gMode);
	glPopMatrix();


	glColor3f (1.0, 1.0, 1.0);
	atorArrive->render(gMode, gResolucao);
	actorFlee->render(gMode, gResolucao);
	actorSeek->render(gMode, gResolucao);
	actorWander->render(gMode, gResolucao);

	// Executa os comandos
	glutSwapBuffers();

	glutPostRedisplay();

}


/*
Fun��o respons�vel pelo desenho da tela
Nesta fun��o s�o determinados o tipo de Proje��o
o modelo de Matrizes e
a posi��o da c�mera
Quando a tela � redimensionada os valores
da vis�o perspectiva s�o recalculados com base no novo tamanho da tela
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

/* Fun��o respons�vel pelo controle de teclado
quando pressionada a tecla d, ser� executada uma rota��o no
pr�prio eixo da esfera menor. Quando pressionada a tecla y
a esfera menor ir� rotacionar em torno da esfera maior, em uma
�rbida determinada na transla��o na fun��o display()
A tecla w � respons�vel por determinar se as esferas ser�o s�lidas
ou aramadas (wire)
*/   

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case 'w':
		case 'W':
			if(gMode == DisplayListElements::WIRED) {
				gMode = DisplayListElements::SOLID;
			} else {
				gMode = DisplayListElements::WIRED;
			}
			break;
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
			gResolucao = (key-'0'<RESOLUCOES?key-'0':RESOLUCOES-1);

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
Fun��o principal do programa.
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
