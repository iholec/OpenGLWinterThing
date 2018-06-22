#include <stdlib.h> // for exit

#include <stdio.h>
#include <iostream>
#include <list>
#include <math.h>
#include <time.h> 
#include <windows.h>

#include "ModelLoaderLib/glew.h"
#include "tga.h"
#include "Object.h"
#include "Galaxy.h"
#include "ModelLoaderLib/mesh.h"

#ifdef __APPLE__
#include <GLUT/glut.h> 
#include <OpenGL/gl.h>  
#include <OpenGL/glu.h>  
#else
#include <GL/glut.h> 
#include <GL/gl.h>  
#include <GL/glu.h>  
#endif


#ifndef GALAXY_AMOUNT
#define GALAXY_AMOUNT 10
#endif

/* some math.h files don't define pi... */
#ifndef M_PI
#define M_PI 3.141592653
#endif

#define RAD(x) (((x)*M_PI)/180.)

#ifdef __STRICT_ANSI__
#define sinf(x) ((float)sin((x)))
#define cosf(x) ((float)cos((x)))
#define atan2f(x, y) ((float)atan2((x), (y)))
#endif 
using namespace std;

int window;
float advanceX = 0.0f;
float advanceZ = 0.0f;
float advanceY = 0.0f;

bool discomode = false;

Mesh* SpiegeleiMesh;

GLuint textures[100];
int numberOfTextures = 7;

int moving = 0;     //flag that is true while mouse moves 
int begin_x = 0;        //x value of mouse movement
int begin_y = 0;      //y value of mouse movement
GLfloat angle_y = 0;  //angle of spin around y axis of scene, in degrees
GLfloat angle_x = 0;  //angle of spin around x axis  of scene, in degrees

Galaxy galaxies[GALAXY_AMOUNT];

float hour = 0.0;
float day = 0.0;
float inc = 1.00;
void initUniverse();

//reports GL Errors
//Copyright CGE2018
void reportGLError(const char * msg)
{
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		fprintf(stderr, "OpenGL Error: %s %s\n", msg, errString);
	}
	return;
}

//resizes everything to match the window size
//Copyright CGE2018
void resize(int width, int height)
{
	// prevent division by zero
	if (height == 0) { height = 1; }

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)width / (float)height, 0.1f, 1201.0f);
	glMatrixMode(GL_MODELVIEW);
}

//Enables the user to Move and Interact with the Rotation Speed 
//as well as create a new Random Galaxy by pressing keys
void keyPressed(unsigned char key, int x, int y)
{

	switch (key) {
	case 27:
		glutDestroyWindow(window);
		exit(0);
		break;
	case 'w':     /* <cursor up> */
		advanceX += 0.1f;
		glutPostRedisplay();
		break;
	case 's':     /* <cursor down> */
		advanceX -= 0.1f;
		glutPostRedisplay();
		break;
	case 'a':
		advanceZ += 0.1f;
		glutPostRedisplay();
		break;
	case 'd':
		advanceZ -= 0.1f;
		glutPostRedisplay();
		break;
	case 'q':
		advanceY += 0.1f;
		glutPostRedisplay();
		break;
	case 'e':
		advanceY -= 0.1f;
		glutPostRedisplay();
		break;
	case 'r':
		inc += 0.1f;
		glutPostRedisplay();
		break;
	case 't':
		inc += 5.0f;
		glutPostRedisplay();
		break;
	case 'f':
		inc -= 0.1f;
		glutPostRedisplay();
		break;
	case 'z':
		initUniverse();
		glutPostRedisplay();
		break;
	case 'y':
		if (discomode) {
			discomode = false;
			PlaySound("Silent.wav", NULL, SND_ASYNC | SND_FILENAME);
			inc = 1.0f;
		}
		else {
			discomode = true;
			PlaySound("disco.wav", NULL, SND_ASYNC | SND_FILENAME);
			inc = 200.0f;
		}
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

//draws a cube with a different texture on every side. Scaleable.
void drawCube(GLuint *texfront, GLuint *texback, GLuint *textop, GLuint *texbottom, GLuint *texright, GLuint *texleft, int Scale)
{
	glBindTexture(GL_TEXTURE_2D, *texfront);
	glBegin(GL_QUADS);
	// front face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f*Scale, -1.0f*Scale, 1.0f*Scale);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f*Scale, -1.0f*Scale, 1.0f*Scale);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f*Scale, 1.0f*Scale, 1.0f*Scale);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f*Scale, 1.0f*Scale, 1.0f*Scale);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, *texback);
	glBegin(GL_QUADS);
	// back face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f*Scale, -1.0f*Scale, -1.0f*Scale);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f*Scale, 1.0f*Scale, -1.0f*Scale);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f*Scale, 1.0f*Scale, -1.0f*Scale);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f*Scale, -1.0f*Scale, -1.0f*Scale);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, *textop);
	glBegin(GL_QUADS);
	// top face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f*Scale, 1.0f*Scale, -1.0f*Scale);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f*Scale, 1.0f*Scale, 1.0f*Scale);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f*Scale, 1.0f*Scale, 1.0f*Scale);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f*Scale, 1.0f*Scale, -1.0f*Scale);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, *texbottom);
	glBegin(GL_QUADS);
	// bottom face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f*Scale, -1.0f*Scale, -1.0f*Scale);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f*Scale, -1.0f*Scale, -1.0f*Scale);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f*Scale, -1.0f*Scale, 1.0f*Scale);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f*Scale, -1.0f*Scale, 1.0f*Scale);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, *texright);
	glBegin(GL_QUADS);
	// right face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f*Scale, -1.0f*Scale, -1.0f*Scale);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f*Scale, 1.0f*Scale, -1.0f*Scale);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f*Scale, 1.0f*Scale, 1.0f*Scale);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f*Scale, -1.0f*Scale, 1.0f*Scale);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, *texleft);
	glBegin(GL_QUADS);
	// left face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f*Scale, -1.0f*Scale, -1.0f*Scale);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f*Scale, -1.0f*Scale, 1.0f*Scale);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f*Scale, 1.0f*Scale, 1.0f*Scale);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f*Scale, 1.0f*Scale, -1.0f*Scale);
	glEnd();
}

//draws a classic Sphere
void drawSphere(GLuint *tex, float size) {

	GLUquadric *qobj = gluNewQuadric();

	gluQuadricTexture(qobj, GL_TRUE);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *tex);

	gluSphere(qobj, size, 20, 20);

	gluDeleteQuadric(qobj);
	glDisable(GL_TEXTURE_2D);
}

//draws a sphere that emittes light
void drawGlowingSphere(GLuint *tex, float size, GLfloat *glowColor) {

	GLUquadric *qobj = gluNewQuadric();

	gluQuadricTexture(qobj, GL_TRUE);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glMaterialfv(GL_FRONT, GL_EMISSION, glowColor);
	gluSphere(qobj, size, 20, 20);

	if (discomode) {
		GLfloat discomode[] = { 0.5 *static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - (-3)))), 0.5 * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - (-3)))), 0.5*static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - (-3)))) };
		glMaterialfv(GL_FRONT, GL_EMISSION, discomode);
	}
	else {
		GLfloat softSun[] = { 0.4, 0.4, 0.4 };
		glMaterialfv(GL_FRONT, GL_EMISSION, softSun);
	}

	//
	//disco mode


	gluDeleteQuadric(qobj);
	glDisable(GL_TEXTURE_2D);
}

//draws the white part of a fried egg (model)
void drawSpiegelei(GLuint *tex) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *tex);
	SpiegeleiMesh->Draw();
	glDisable(GL_TEXTURE_2D);
}

//draws and animates because it is called every frame
void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);



	gluLookAt(-sinf(RAD(angle_y)), sinf(RAD(angle_x)), cosf(RAD(angle_y)),
		0., 0., 0.,
		0., 1., 0.);

	drawCube(&textures[11], &textures[9], &textures[12], &textures[13], &textures[10], &textures[8], 150);

	glTranslatef(advanceZ, advanceY, advanceX);

	hour += inc;
	day += inc / 24.0;
	hour = hour - ((int)(hour / 24)) * 24;
	day = day - ((int)(day / 365)) * 365;

	glTranslatef(0.0, 0.0, -50.0);

	glRotatef(360 * day / 365.0, 0.0, 1.0, 0.0);

	// ecliptic
	glRotatef(15.0, 1.0, 0.0, 0.0);


	for (int i = 0; i < GALAXY_AMOUNT; i++)
	{
		glPushMatrix();
		Galaxy g = galaxies[i];
		glTranslatef(g.centerX, g.centerY, g.centerZ);
		
		GLfloat sunLight[] = { 1,1,0 };
		drawGlowingSphere(&textures[15], 0.5, sunLight);
		drawSpiegelei(&textures[16]);
		for (int i = 0; i < 10; i++)
		{
			Object p = g.planets[i];
			Object center = g.planets[p.rotateAround];
			glRotatef(360.0*day / 365.0, g.rotaX, g.rotaY, g.rotaZ);
			glPushMatrix();

			glTranslatef(center.posX, center.posY, center.posZ);
			glRotatef(360.0*day / 365.0, p.rotaX1, p.rotaY1, p.rotaZ1);


			glTranslatef(p.posX, p.posY, p.posZ);
			glRotatef(360.0*day / 24.0, p.rotaX2, p.rotaY2, p.rotaZ2);
			drawSphere(&textures[p.texture], p.size);

			glPopMatrix();
		}
		glPopMatrix();

	}

	glutSwapBuffers();

}

//initializes a random Universe with a fried egg Sun
//adds the planets to the galaxies planet array
void initUniverse() {

	SpiegeleiMesh = new Mesh("./Models/Fried_Egg/ShysSpiegeleil2.OBJ");
	inc = 1.0;

	for (int i = 0; i < GALAXY_AMOUNT; i++)
	{
		Galaxy gal;
		for (int i2 = 0; i2 < GALAXY_AMOUNT; i2++)
		{

			Object planet;
			planet.texture = rand() % (numberOfTextures);
			planet.rotateAround = 0;
			cout << planet.texture;

			gal.planets[i2] = planet;
		}

		galaxies[i] = gal;

	}
}

//loads and initializes a tga texture
//with help from the tga.h loader
int initTexture(char* picture, GLuint* tex) {
	GLsizei w, h;
	tgaInfo *info = 0;
	int mode;


	info = tgaLoad(picture);

	if (info->status != TGA_OK) {
		fprintf(stderr, "error loading texture image: %d\n", info->status);

		return 0;
	}


	mode = info->pixelDepth / 8;  // will be 3 for rgb, 4 for rgba
	glGenTextures(1, tex);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	// Upload the texture bitmap. 
	w = info->width;
	h = info->height;

	reportGLError("before uploading texture");
	GLint format = (mode == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format,
		GL_UNSIGNED_BYTE, info->imageData);
	reportGLError("after uploading texture");

	tgaDestroy(info);
	return 1;
}

//fills variables and starts everything
//mostly just texture loading
void init(int width, int height)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	resize(width, height);

	initTexture("Textures/steak512.tga", &textures[0]);
	initTexture("Textures/cheese.tga", &textures[1]);
	initTexture("Textures/pasta.tga", &textures[2]);
	initTexture("Textures/fettuccinePasta.tga", &textures[3]);
	initTexture("Textures/sphagettiPasta.tga", &textures[4]);
	initTexture("Textures/bowtiePasta.tga", &textures[5]);
	initTexture("Textures/catBiscuit.tga", &textures[6]);

	//skybox
	initTexture("Textures/skyboxLeft1.tga", &textures[8]);
	initTexture("Textures/skyboxLeft2.tga", &textures[9]);
	initTexture("Textures/skyboxRight1.tga", &textures[10]);
	initTexture("Textures/skyboxRight2.tga", &textures[11]);
	initTexture("Textures/skyboxTop.tga", &textures[12]);
	initTexture("Textures/skyboxBottom.tga", &textures[13]);

	//Spiegelei
	initTexture("Textures/eggYellow.tga", &textures[15]);
	initTexture("Textures/eggWhite.tga", &textures[16]);


}

//times
//Copyright CGE2018
void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(15, timer, 1);
}

//inits mouse movement
//Copyright CGE2018
void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:    /* spin scene around */
		if (state == GLUT_DOWN) {
			moving = 1;
			begin_x = x;
			begin_y = y;

		}
		else if (state == GLUT_UP) {
			moving = 0;
		}
		break;

	default:
		break;
	}

	glutPostRedisplay();
}

//calculates mouse movement
//Copyright CGE2018
void mouseMotion(int x, int y) {

	if (moving) { /* mouse button is pressed */

				  /* calculate new modelview matrix values */
		angle_y = angle_y + (x - begin_x);
		angle_x = angle_x + (y - begin_y);
		if (angle_x > 360.0) angle_x -= 360.0;
		else if (angle_x < -360.0) angle_x += 360.0;
		if (angle_y > 360.0) angle_y -= 360.0;
		else if (angle_y < -360.0) angle_y += 360.0;

		begin_x = x;
		begin_y = y;
		glutPostRedisplay();
	}
}

//classic main
//Copyright CGE2018
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);
	window = glutCreateWindow("Food Galaxy");
	glutDisplayFunc(&display);
	glutReshapeFunc(&resize);
	glutKeyboardFunc(&keyPressed);
	init(640, 480);
	srand(time(NULL));
	initUniverse();
	glutTimerFunc(15, timer, 1);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutFullScreen();
	glutMainLoop();
	return 0;
}