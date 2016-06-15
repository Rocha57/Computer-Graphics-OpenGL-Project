//COMPILE
//g++ main.cpp RgbImage.cpp -lGL -lglut -lGLU -o main

//For windows
//#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "RgbImage.h"


//--------------------------------- Definir cores
#define AZUL     0.0, 0.0, 1.0, 1.0
#define VERMELHO 1.0, 0.0, 0.0, 1.0
#define AMARELO  1.0, 1.0, 0.0, 1.0
#define VERDE    0.0, 1.0, 0.0, 1.0
#define LARANJA  1.0, 0.5, 0.1, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY     0.9, 0.92, 0.29, 1.0
#define PI		 3.14159

//================================================================================
//===========================================================Variaveis e constantes

//------------------------------------------------------------ Sistema Coordenadas
GLfloat   corner = 15.0;
GLint     wScreen=800, hScreen=600;
GLfloat   mesa=3.0;
GLfloat   bule=1.3;
GLfloat   quad=6.0;
GLfloat   mesaP[]= {4, 0, 10};
GLfloat   buleP[]= {4, 0, 10};
GLfloat   quadP[]= {4, 4, 1};


//------------------------------------------------------------ Observador
GLint    defineView=0;
GLint    defineProj=1;
GLfloat  raio   = 1;
GLfloat  angulo = 0.35*PI;

//====== Coordenadas do observador e coordenadas para onde ele olha

GLfloat  obsP[] = {0,3,0};
GLfloat	 olharPara[] = {raio*cos(angulo), 3, raio*sin(angulo)};


GLfloat  incy   = 0.5;
GLfloat  inca   = 0.03;
GLfloat  angBule = 0;
GLfloat  incBule = 10;

//------------------------------------------------------------ Texturas
GLint    repete=2;
GLfloat  rr=1;
GLint    maxR  =20;
GLint    numFrames =5;              //numero de imagens a colocar em loop na tela
GLint    msec=100;					//.. definicao do timer (actualizacao)


//------------------------------------------------------------ Texturas
GLuint  texture[4];
RgbImage imag;


void criaDefineTexturas()
{


	//----------------------------------------- Chao y=0
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("chao.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	//----------------------------------------- Parede z=0
	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("parede.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	
}


void init(void)
{
	glClearColor(BLACK);
	glShadeModel(GL_SMOOTH);
	
	criaDefineTexturas();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

}


void resizeWindow(GLsizei w, GLsizei h)
{
 	wScreen=w;
	hScreen=h;
	//glViewport( 0, 0, wScreen,hScreen );
	//glutReshapeWindow(wScreen,hScreen);
	glutPostRedisplay();
}


void drawScene(){
	//==============================================================
	//== Desenha cenas e aplica as texturas necessárias

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Chao y=0
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( -corner,  0, -corner );
			glTexCoord2f(10.0f,0.0f); glVertex3i( corner, 0, -corner );
			glTexCoord2f(10.0f,10.0f); glVertex3i( corner, 0, corner);
			glTexCoord2f(0.0f,10.0f); glVertex3i( -corner,  0,  corner);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Parede z negativa
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( -corner,  0, -corner );
			glTexCoord2f(1.0f,0.0f); glVertex3i( corner, 0, -corner );
			glTexCoord2f(1.0f,1.0f); glVertex3i( corner, corner, -corner);
			glTexCoord2f(0.0f,1.0f); glVertex3i( -corner,  corner,  -corner);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//----------------------------------------------------- Parede z positiva
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( corner,  0, corner );
			glTexCoord2f(1.0f,0.0f); glVertex3i( -corner, 0, corner );
			glTexCoord2f(1.0f,1.0f); glVertex3i( -corner, corner, corner);
			glTexCoord2f(0.0f,1.0f); glVertex3i( corner,  corner,  corner);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Parede x negativa
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( -corner,  0, corner);
			glTexCoord2f(1.0f,0.0f); glVertex3i( -corner, 0, -corner );
			glTexCoord2f(1.0f,1.0f); glVertex3i( -corner, corner, -corner);
			glTexCoord2f(0.0f,1.0f); glVertex3i( -corner,  corner,  corner);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Parede x positiva
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( corner,  0, -corner );
			glTexCoord2f(1.0f,0.0f); glVertex3i( corner, 0, corner );
			glTexCoord2f(1.0f,1.0f); glVertex3i( corner, corner, corner);
			glTexCoord2f(0.0f,1.0f); glVertex3i( corner,  corner,  -corner);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixos
	glColor4f(WHITE);
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0);
		glVertex3i(10, 0, 0);
	glEnd();
	glBegin(GL_LINES);
		glVertex3i(0,  0, 0);
		glVertex3i(0, 10, 0);
	glEnd();
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0);
		glVertex3i( 0, 0,10);
	glEnd();

}

void display(void){

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Apagar ]
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Janela Visualizacao ]
	glViewport (0,0,wScreen, hScreen);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Projeccao]
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//GluPerspective vai ser tridimensional
	gluPerspective(88.0, wScreen/hScreen, 0.1, 3*corner); 
	

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Modelo+View(camera/observador) ]
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(obsP[0], obsP[1], obsP[2], olharPara[0],olharPara[1],olharPara[2], 0, 2, 0);
	

	criaDefineTexturas();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Objectos ]
	drawScene();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Actualizacao
	glutSwapBuffers();
}


void Timer(int value)
{

	angBule=angBule+incBule;
	glutPostRedisplay();
	glutTimerFunc(msec,Timer, 1);
}

//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y){

	switch (key) {

	//--------------------------- Textura no papel de parede
	case 't':
	case 'T':
		glutPostRedisplay();
		break;
		
	
	//---------------------------- Observador
	case 'o':
		obsP[0] = 0;
		obsP[1] = 3;
		obsP[2] = 0;
		glutPostRedisplay();
		break;
	case 49:
		obsP[0] = 0;
		obsP[1] = 3;
		obsP[2] = corner;

		glutPostRedisplay();
		break;

	//--------------------------- Escape
	case 27:
		exit(0);
		break;
  }
}

void teclasNotAscii(int key, int x, int y){
    if(key == GLUT_KEY_UP)
		obsP[1]=obsP[1]+incy;
		if(key == GLUT_KEY_DOWN)
		obsP[1]=obsP[1]-incy;
	if(key == GLUT_KEY_LEFT)
		angulo = angulo+inca;
	if(key == GLUT_KEY_RIGHT)
		angulo = angulo-inca;

		olharPara[0] = cos(angulo);
		
		olharPara[2] = sin(angulo);
//=======
    	olharPara[0] = raio*cos(angulo);
		olharPara[2] = raio*sin(angulo);


	glutPostRedisplay();
}

//======================================================= MAIN
int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize (wScreen, hScreen);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("{pjmm,jpa}@dei.uc.pt-CG ::::::::::::::: (left,right,up,down, 'q', 't)' ");

	init();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}
