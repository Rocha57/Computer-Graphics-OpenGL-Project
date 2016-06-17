//COMPILE
//LINUX
//g++ main.cpp RgbImage.cpp -lGL -lglut -lGLU -o main
//APPLE
//g++ RgbImage.cpp main.cpp -framework OpenGL -framework GLUT -o main

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
#include "materiais.h"


//--------------------------------- Definir cores
#define AZUL     0.0, 0.0, 1.0, 1.0
#define VERMELHO 1.0, 0.0, 0.0, 1.0
#define AMARELO  1.0, 1.0, 0.0, 1.0
#define VERDE    0.0, 1.0, 0.0, 1.0
#define LARANJA  1.0, 0.5, 0.1, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY     0.9, 0.92, 0.29, 1.0
#define BROWN    0.545098, 0.270588, 0.0745098, 1.0
#define PI		 3.14159

//================================================================================
//===========================================================Variaveis e constantes

//------------------------------------------------------------ Sistema Coordenadas
GLfloat   corner = 20.0;
GLfloat   altura = 25.0;
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

GLfloat  obsP[] = {0,10.0,0};
GLfloat	 olharPara[] = {raio*cos(angulo), 3, raio*sin(angulo)};

//=======
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


//------------------------------------------------------------ Materiais
GLfloat matAmbiente[] = {1.0,1.0,1.0,1.0};	  
GLfloat matDifusa[]   = {1.0,1.0,1.0,1.0};	  
GLfloat matEspecular[]= {1.0, 1.0, 1.0, 1.0}; 


//===================== Lights =================//

//Define luz ambiente
GLfloat luzAmbienteCor[4]={1.0,1.0,1.0,1.0}; 

//Localização do candeeiro
GLfloat candeeiroPos[4] ={0.0, 25.0, 0.0, 1.0};

//Define um "candeeiro"
//Definição da luz "simples" / "branca";
GLfloat luzCandeeirolCor[4]={0.1, 0.1, 0.1, 1.0}; 

//Caracteristicas do candeeiro da atenuação atmosférica
GLfloat candeeiroAttCon =1.0;
GLfloat candeeiroAttLin =0.05;
GLfloat candeeiroAttQua =0.0;

GLint ligaLuz = 0;


//==============Init lights================//

void initLights(){
	//Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbienteCor);

	//Candeeiro
	glLightfv(GL_LIGHT0, GL_POSITION, candeeiroPos );
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzCandeeirolCor );
	glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, candeeiroAttCon);
	glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, candeeiroAttLin);
	glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION,candeeiroAttQua);


}


void criaDefineTexturas()
{

	// Table TOP
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("tampo_mesa.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());


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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("1545.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

}

void initMaterials(int material)
{
	switch(material){
		case 1:
			glMaterialfv(GL_FRONT,GL_AMBIENT,  redPlasticAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  redPlasticDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, redPlasticSpec);
			glMateriali (GL_FRONT,GL_SHININESS,redPlasticCoef);
			break;
		case 2:
			glMaterialfv(GL_FRONT,GL_AMBIENT,  greenPlasticAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  greenPlasticDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, greenPlasticSpec);
			glMateriali (GL_FRONT,GL_SHININESS,greenPlasticCoef);
			break;
		case 3:
			glMaterialfv(GL_FRONT,GL_AMBIENT,  cyanPlasticAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  cyanPlasticDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, cyanPlasticSpec);
			glMateriali (GL_FRONT,GL_SHININESS,cyanPlasticCoef);
			break;
		case 4:
			glMaterialfv(GL_FRONT,GL_AMBIENT,  yellowPlasticAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  yellowPlasticDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, yellowPlasticSpec);
			glMateriali (GL_FRONT,GL_SHININESS,yellowPlasticCoef);
			break;
		case 5:
			glMaterialfv(GL_FRONT,GL_AMBIENT,  goldAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  goldDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, goldSpec);
			glMateriali (GL_FRONT,GL_SHININESS,goldCoef);
			break;
	}

}

void init(void)
{
	glClearColor(BLACK);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	criaDefineTexturas();
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

	initMaterials(5);

	initLights();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}


void resizeWindow(GLsizei w, GLsizei h)
{
 	wScreen=w;
	hScreen=h;
	//glViewport( 0, 0, wScreen,hScreen );
	//glutReshapeWindow(wScreen,hScreen);
	glutPostRedisplay();
}

void drawPlayers(){

	GLint i = 0;
	GLfloat players[4][3] = {{10.0, 5.0, 0.0}, {0.0, 5.0, -10.0}, {-10.0, 5.0, 0.0}, {0.0, 5.0, 10.0}};

	//glColor4f(WHITE);
	
	//Usar Materiais
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	
	initLights();
	
	initMaterials(5);

	for (i = 0; i <= 3; i++)
	{
		glPushMatrix();
			glTranslatef(players[i][0], players[i][1], players[i][2]);
			glScalef(1.0,5.0,1.0);
			glutSolidCube(2.0);
			
		glPopMatrix();
	}

	//Cancela Materiais
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
}

//======================================================================== ILUMINCCAO
void iluminacao(){
	
	if (ligaLuz)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);

}

void drawChips(){
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	
	initLights();
	
	initMaterials(1);
	//glColor4f(VERMELHO);
	glPushMatrix();
		glTranslatef(-6.5,4.2,0.0);
		glRotatef (90, -1, 0, 0);
		GLUquadricObj* quadobj3 = gluNewQuadric();
		gluQuadricDrawStyle ( quadobj3, GLU_FILL   );
		gluQuadricNormals   ( quadobj3, GLU_SMOOTH );
		gluDisk(quadobj3, 0, 0.5, 100, 100);
		GLUquadricObj* quadobj4 = gluNewQuadric();
		gluQuadricDrawStyle ( quadobj4, GLU_FILL   );
		gluQuadricNormals   ( quadobj4, GLU_SMOOTH );
		gluCylinder(quadobj4, 0.5, 0.5, 0.2, 100, 100);
	glPopMatrix();
	initMaterials(2);
	//glColor4f(VERDE);
	glPushMatrix();
		glTranslatef(6.5,4.2,0.0);
		glRotatef (90, -1, 0, 0);
		GLUquadricObj* quadobj5 = gluNewQuadric();
		gluQuadricDrawStyle ( quadobj5, GLU_FILL   );
		gluQuadricNormals   ( quadobj5, GLU_SMOOTH );
		gluDisk(quadobj5, 0, 0.5, 100, 100);
		GLUquadricObj* quadobj6 = gluNewQuadric();
		gluQuadricDrawStyle ( quadobj6, GLU_FILL   );
		gluQuadricNormals   ( quadobj6, GLU_SMOOTH );
		gluCylinder(quadobj6, 0.5, 0.5, 0.2, 100, 100);
	glPopMatrix();
	initMaterials(3);
	//glColor4f(AZUL);
	glPushMatrix();
		glTranslatef(0.0,4.2,-6.5);
		glRotatef (90, -1, 0, 0);
		GLUquadricObj* quadobj7 = gluNewQuadric();
		gluQuadricDrawStyle ( quadobj7, GLU_FILL   );
		gluQuadricNormals   ( quadobj7, GLU_SMOOTH );
		gluDisk(quadobj7, 0, 0.5, 100, 100);
		GLUquadricObj* quadobj8 = gluNewQuadric();
		gluQuadricDrawStyle ( quadobj8, GLU_FILL   );
		gluQuadricNormals   ( quadobj8, GLU_SMOOTH );
		gluCylinder(quadobj8, 0.5, 0.5, 0.2, 100, 100);
	glPopMatrix();
	initMaterials(4);
	//glColor4f(AMARELO);
	glPushMatrix();
		glTranslatef(0.0,4.2,6.5);
		glRotatef (90, -1, 0, 0);
		GLUquadricObj* quadobj9 = gluNewQuadric();
		gluQuadricDrawStyle ( quadobj9, GLU_FILL   );
		gluQuadricNormals   ( quadobj9, GLU_SMOOTH );
		gluDisk(quadobj9, 0, 0.5, 100, 100);
		GLUquadricObj* quadobj10 = gluNewQuadric();
		gluQuadricDrawStyle ( quadobj10, GLU_FILL   );
		gluQuadricNormals   ( quadobj10, GLU_SMOOTH );
		gluCylinder(quadobj10, 0.5, 0.5, 0.2, 100, 100);
	glPopMatrix();

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
}

void drawScene(){
	//==============================================================
	//== Desenha cenas e aplica as texturas necessárias
	//== Tabela passou a ser desenhada com cilindros

	drawChips();

	//Draw Table Leg

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
		glRotatef (90, -1, 0, 0);
		
		GLUquadricObj* quadobj1 = gluNewQuadric();
		gluQuadricDrawStyle ( quadobj1, GLU_FILL   );
		gluQuadricNormals   ( quadobj1, GLU_SMOOTH );
		gluQuadricTexture	(quadobj1, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, texture[1] );
		gluCylinder(quadobj1, 1, 0, 3.5, 100, 100);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	
	//Draw Table Top
	glPushMatrix();
		glTranslatef(0,4,0);
		glRotatef(90,1,0,0);
		GLUquadricObj* quadobj2 = gluNewQuadric();
		gluQuadricTexture(quadobj2, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, texture[0] );
		gluCylinder(quadobj2, 7.5, 0, 0.5, 100, 100);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);


	//Draw Table Limits

	glColor4f(BROWN);
	glPushMatrix();
		glTranslatef(0,4.2,0);
		glRotatef(90,1,0,0);
		GLUquadricObj*  y = gluNewQuadric ( );
		gluQuadricDrawStyle ( y, GLU_FILL   );
		gluQuadricNormals   ( y, GLU_SMOOTH );
		gluCylinder ( y, 7.5, 7.5, 0.5,150,150);
	glPopMatrix();
	


	//==========================CAIXA MUNDO==================//

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Chao y=0
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( -corner,  0, -corner );
			glTexCoord2f(1.0f,0.0f); glVertex3i( corner, 0, -corner );
			glTexCoord2f(1.0f,1.0f); glVertex3i( corner, 0, corner);
			glTexCoord2f(0.0f,1.0f); glVertex3i( -corner,  0,  corner);
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
			glTexCoord2f(1.0f,1.0f); glVertex3i( corner, altura, -corner);
			glTexCoord2f(0.0f,1.0f); glVertex3i( -corner,  altura,  -corner);
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
			glTexCoord2f(1.0f,1.0f); glVertex3i( -corner, altura, corner);
			glTexCoord2f(0.0f,1.0f); glVertex3i( corner,  altura,  corner);
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
			glTexCoord2f(1.0f,1.0f); glVertex3i( -corner, altura, -corner);
			glTexCoord2f(0.0f,1.0f); glVertex3i( -corner,  altura,  corner);
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
			glTexCoord2f(1.0f,1.0f); glVertex3i( corner, altura, corner);
			glTexCoord2f(0.0f,1.0f); glVertex3i( corner,  altura,  -corner);
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

	//== Draw players
	drawPlayers();

	glutPostRedisplay();

}


void display(void){

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[ Apagar ]
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//≃================= Luzes
	glEnable(GL_LIGHTING);

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

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ [Iluminuação]
	iluminacao();

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

	case 'l':
		ligaLuz=!ligaLuz;
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
	glutCreateWindow ("CG Project");

	init();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}
