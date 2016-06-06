/* ===================================================================================
	Departamento Eng. Informatica - FCTUC
	Computacao Grafica - 2015/16
	................................................... PMartins/JArrais
	Trabalho 0 - Introducao ao OpenGL
======================================================================================= */


//.................................................... Bibliotecas necessarias
#include <stdlib.h>
#include "stdio.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

//.................................................... Variaveis
GLfloat r,g,b;						// cor RGB
GLfloat xx,yy;

//Angulo para rodar a figura
GLfloat angle;


//-----------------------------------------------------------------------------------
//																		Init
//-----------------------------------------------------------------------------------
void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 1.0);	//....	Cor para apagar (Branco)
	gluOrtho2D(-400, 400, -400, 400);		//....	Definicao sistema coordenadas area de desenho
	glShadeModel(GL_FLAT);				//....  Interpolacao de cor com base na cor dos vertices
	r=1; g=0; b=0;						//....  Vermelho
	xx=0; yy=0;
	angle = 0;
}

void keyboard(unsigned char key, int x, int y){

	switch (key) {
	case 'r':
		r=1.0; g=0.0; b=0.0;
		glutPostRedisplay();
		break;

	case 'g':
		r=0.0; g=1.0; b=0.0;
		glutPostRedisplay();
		break;

	case 'b':
		r=0.0; g=0.0; b=1.0;
		glutPostRedisplay();
		break;

	case 27:					//tecla ESCAPE
		exit(0);
		break;
	}
}

void onMouseClick(int button, int state, int x, int y){

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
    //Manda para a consola. É tipo um log de debug de merdas xD
    printf("Mouse Clicked\n");
    angle += 10;
    printf("Angle: %0.2f\n", angle);
    glutPostRedisplay();
  }
}

void movement(int key, int x, int y){
	switch (key){
		case GLUT_KEY_UP:
			yy+=10.0f;
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			yy-=10.0f;
			glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
			xx-=10.0f;
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			xx+=10.0f;
			glutPostRedisplay();
			break;

	}
}

//---------------------------------------- Fun�cao callback de desenho (principal)
void desenhaEixoCartesiano(void)
{
  //Desenhar o eixo carteziano
  glPushMatrix();
  glColor3f (0, 0, 0);
  glLineWidth(65);
  glBegin(GL_LINES);
    glVertex2f(0,-400);
    glVertex2f(0,400);

    glVertex2f(-400,0);
    glVertex2f(400,0);
  glEnd();
  glPopMatrix();
}

void desenhaPoligono(void){
  //Desenhar o poligono
  glPushMatrix();

  glColor3f(r,g,b);
  glRotatef(angle, 0, 0, 1);

 	glBegin(GL_POLYGON);			//.... Desenha poligno
 		glVertex2f(50+xx,50+yy);      //.... Especifica os v�rtices
	 	glVertex2f(100+xx,50+yy);
		glVertex2f(100+xx,100+yy);
	 	glVertex2f(50+xx,100+yy);
 	glEnd();

  glPopMatrix();
}

void draw(void) {

  glClear(GL_COLOR_BUFFER_BIT);			//....	Apaga ecra

  desenhaEixoCartesiano();
  desenhaPoligono();

  glutSwapBuffers();					//.. actualiza ecran
}


//-----------------------------------------------------------------------------------
//																		         MAIN
//-----------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	glutInit(&argc, argv);							//===1:Inicia janela
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);   //		:Double mode, RBG
  glutInitWindowSize(600,500);					//		:dimensoes (pixeis)
  glutInitWindowPosition(200,100);				//		:localizacao da janela
	glutCreateWindow("Exemplo inicial");           //      :criacao da janela

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	init();									//===2:Inicia estado/parametros

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
													//===3:Definicao callbaks
	glutDisplayFunc(draw);						//		:desenho
	glutKeyboardFunc(keyboard);						//		:eventos teclado
  glutMouseFunc(onMouseClick);
	glutSpecialFunc(movement);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	glutMainLoop();								//===4:Inicia processamento


    return 0;
}
