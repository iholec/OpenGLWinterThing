//extern annotation versuchen wegen Linker


#include "ModelLoaderLib/OBJ_Loader.h"
#include <stdlib.h> // for exit
#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <windows.h>

#include "tga.h"
#include "Coordinate.h"
#include "ParticleSystem.h"

#ifdef __APPLE__
#include <GLUT/glut.h> 
//#include <OpenGL/gl.h>  
#include <OpenGL/glu.h>  
#else
#include <GL/glut.h> 
//#include <GL/gl.h>  
#include <GL/glu.h>  
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

ParticleSystem* particleSystem;

int window;
float advanceX = 0.0f;
float advanceZ = 0.0f;
float advanceY = 0.0f;

int rot = 0;

bool discomode = false;

const int ISLAND_AMOUNT = 5;
coordinate ISLAND_COORDINATES[ISLAND_AMOUNT];


objl::Loader loader;

GLuint textures[100];
int numberOfTextures = 7;

int moving = 0;     //flag that is true while mouse moves 
int begin_x = 0;        //x value of mouse movement
int begin_y = 0;      //y value of mouse movement
GLfloat angle_y = 0;  //angle of spin around y axis of scene, in degrees
GLfloat angle_x = 0;  //angle of spin around x axis  of scene, in degrees

float inc = 1.00;

const coordinate camera(0, 0, 0);

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
	case 'g':
		particleSystem->setGravity(!particleSystem->getGravity());
		break;
	case 'y':
		if (discomode) {
			discomode = false;
			PlaySound("Sound/ambientMusic.wav", NULL, SND_ASYNC | SND_FILENAME);
			particleSystem->setDiscomode(discomode);
		}
		else 
		{
			discomode = true;
			PlaySound("Sound/disco.wav", NULL, SND_ASYNC | SND_FILENAME);
			particleSystem->setDiscomode(discomode);
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

//draws a model
void drawMesh(int meshIndex) {
	glBegin(GL_TRIANGLES);
	for (auto& vertex : loader.LoadedMeshes[meshIndex].Vertices)
	{
		glTexCoord2f(vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y);
		glVertex3f(vertex.Position.X, vertex.Position.Y, vertex.Position.Z);
	}
	glEnd();
}

void modelTexture(int textureIndex)
{
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[textureIndex]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}


int lod_level(const float x, const float y, const float z)
{
	const coordinate model(x, y, z);

	const coordinate distance(model.x - camera.x, model.y - camera.y, model.z - camera.z);

	const auto distance_len = sqrt((distance.x *distance.x) + (distance.y *distance.y) + (distance.z *distance.z)); //todo remove sqrt

	if (distance_len < 25)
	{
		return 0;
	}
	else if (distance_len >= 25 && distance_len < 75)
	{
		return 1;
	}
	else
	{
		return 2;
	}
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

	//skybox
	drawCube(&textures[6], &textures[4], &textures[7], &textures[8], &textures[5], &textures[3], 300);

	glTranslatef(advanceZ, advanceY, advanceX);

	//Island
	glPushMatrix();
	glTranslatef(-200.0, -50.0, -200.0);
	if(discomode)
	{
		glRotatef(rot % 360, 0.0, 0.0, 0.0);
		rot++;
	}
	for (coordinate pos : ISLAND_COORDINATES)
	{
		glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		glScalef(4.0f, 4.0f, 4.0f);

		modelTexture(2);
		drawMesh(1);
		glDisable(GL_TEXTURE_2D);
		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		int LOD_LEVEL = lod_level(matrix[12], matrix[13], matrix[14]);
		if (LOD_LEVEL == 0)
		{
			modelTexture(1);
			drawMesh(3);
			glDisable(GL_TEXTURE_2D);
		}
		else if (LOD_LEVEL >= 2)
		{
			modelTexture(1);
			drawMesh(2);
			glDisable(GL_TEXTURE_2D);
		}
		else if (LOD_LEVEL == 1)
		{
			modelTexture(1);
			drawMesh(0);
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();
	}
	glPopMatrix();

	glEnable(GL_BLEND);

	//oh no
	glPushMatrix();
	glTranslatef(-200.0, 300.0, -200.0);
	particleSystem->update();//it did not kill me!!!
	glPopMatrix();

	glDisable(GL_BLEND);

	glutSwapBuffers();


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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
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

void create_positions()
{
	ISLAND_COORDINATES[0] = coordinate(195.0, 45.0, 200.0);

	for (int i = 1; i < ISLAND_AMOUNT; i++)
	{
		const int scaleX = 100;
		const int scaleY = 10;
		const int scaleZ = 100;
		const float pos_x = rand() % (-1 + scaleX - 1 + scaleX + 1) + 1 + scaleX;
		const float pos_y = rand() % (-1 + scaleY - 1 + scaleY + 1) + 1 + scaleY;
		const float pos_z = rand() % (-1 + scaleZ - 1 + scaleZ + 1) + 1 + scaleZ;
		const coordinate coordinate(pos_x, pos_y, pos_z);
		ISLAND_COORDINATES[i] = coordinate;
	}
}

//fills variables and starts everything
//mostly just texture loading
void init(int width, int height)
{
	PlaySound("Sound/wait.wav", NULL, SND_ASYNC | SND_FILENAME);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	resize(width, height);

	//Tree
	initTexture("Textures/bark.tga", &textures[1]);

	//terrain
	initTexture("Textures/SnowFloor.tga", &textures[2]);

	//skybox
	initTexture("Textures/Skybox/Skybox1.tga", &textures[3]);
	initTexture("Textures/skybox/Skybox3.tga", &textures[4]);
	initTexture("Textures/skybox/Skybox4.tga", &textures[5]);
	initTexture("Textures/skybox/Skybox6.tga", &textures[6]);
	initTexture("Textures/skybox/Skybox5.tga", &textures[7]);
	initTexture("Textures/skybox/Skybox2.tga", &textures[8]);

	//Schnee
	initTexture("Textures/snow.tga", &textures[9]);

	//ParticleSystem(int scaleX, int scaleY, int scaleZ, int maxParticles, float minParticleSize, float maxParticleSize, float maxParticleLifetime, float minParticleLifetime, float floatingspeed, GLuint* tex);
	particleSystem = new ParticleSystem(100, 5, 100, 250, 0.3, 1, 500, 100, 0.1, &textures[9]);
	//particleSystem = new ParticleSystem(100, 5, 100, 1000, 1, 3, 500, 500, 0.1, &textures[9]);


	loader.LoadFile("./Models/Island_With_Trees_edit.obj"); //0-> LOD1; 1->Island; 2-> tree_LOD2; 3-> tree_LOD0


	//Light 
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 5.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	//music
	PlaySound("Sound/ambientMusic.wav", NULL, SND_ASYNC | SND_FILENAME);

	//snow
	particleSystem->init(true, true);

	//init islands
	create_positions();
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
	window = glutCreateWindow("Winter Wonderland");
	glutDisplayFunc(&display);
	glutReshapeFunc(&resize);
	glutKeyboardFunc(&keyPressed);
	init(640, 480);
	srand(time(NULL));
	glutTimerFunc(15, timer, 1);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	//glutFullScreen();
	glutMainLoop();
	return 0;
}