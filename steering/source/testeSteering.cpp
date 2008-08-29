#include <windows.h>		// Header File For Windows
#include <math.h>			// Header File For Windows Math Library
#include <stdio.h>			// Header File For Standard Input/Output
#include <time.h>
#include <stdarg.h>
#include <GL/gl.h>
#include <GL/glut.h>


#include "base/Frames.h"
#include "base/GLFont.h"
#include "steering/Actor.h"

GLFont	* font;
Frames  * frames;
double     fps = 100; 
clock_t t1, t2;  

int posicaoluz = 0;
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

Actor * ator;

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
	ator = new Actor();
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


	printf("                 \r", fps);
	printf("FPS: %.0f\t frame_time: %.8f\r", fps, frame_time);

	//static char text[50];
	//font->startText();
	//glColor3f(1,1,1);
	//
	//sprintf(text, "FPS:  %.0f", fps);
	//font->print(20, 460, text);
	//font->endText(); 


	//ator->update(frame_time, Vec3(1,0,0), WANDER);
	//ator->pos.sphericalWrapAround(Vec3(0,0,0), 20);

	//glPushMatrix();
	//ator->render();
	//glPopMatrix();

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
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluLookAt( 10, 10, 10, 0, 0, 0, 0, 1, 0 );

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
	glutCreateWindow ("Teste Steering");
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
