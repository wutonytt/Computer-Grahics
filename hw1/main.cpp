#include "Object.h"
#include "glut.h"
#include <math.h>

#define pi acos(-1)

int windowSize[2];

void light();
void display();
void reshape(GLsizei, GLsizei);
void LoadModel(Object*);
void DrawBasis();
void keyboard(unsigned char key, int x, int y);

Object* elephant = new Object("elephant.obj");
Object* cube = new Object("cube.obj");
Object* teapot = new Object("teapot.obj");

// self-added vars
// for rotate objects
int angle_whole = 0;
int angle_elephant = 0;
int angle_cube = 0;
// for rotate camera
int angle_camera_verticle = 0;
float angle_camera_horizontal = 0.0f;
float lx = 0.0f, lz = 0.0f;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	// light();
	glutCreateWindow("HW1");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

void light()
{
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_position[] = { 50.0, 75.0, 80.0, 1.0 };
	glShadeModel(GL_SMOOTH);
	// z buffer enable
	glEnable(GL_DEPTH_TEST);
	// enable lighting
	glEnable(GL_LIGHTING);
	// set light property
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	///TO DO Enable DepthTest
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_DEPTH_BUFFER_BIT);

	///TO DO: add lighting, Viewport Matrix, Projection Matrix, ModelView Matrix
	/// Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1.0f, 1.0f, 1000.0f);
	
	/// Viewport Matrix
	glViewport(0, 0, 500, 500);
	
	/// ModelView Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(20.0f + angle_camera_horizontal, 20.0f + angle_camera_verticle, 20.0f - abs(angle_camera_verticle/3) - abs(angle_camera_horizontal/3), 0.0f, 0.0f , 0.0f, 0.0f, 1.0f, 0.0f);
	
	light();

	///TO DO: add model, translate, rotate, scale
	glRotatef(angle_whole, 0, 1, 0);

	glPushMatrix();

	DrawBasis();
	
	glTranslatef(3.0f, 0.0f, -3.0f);
	glRotatef(angle_elephant, 0, 1, 0);
	LoadModel(elephant); 

	glTranslatef(-2.0f, 0.0f, -2.0f);
	glTranslatef(0.0f, 0.5f, 0.0f);
	glRotatef(angle_cube, 0, 1, 0);
	glScalef(0.5f, 0.5f, 0.5f);
	LoadModel(cube);

	glPopMatrix();
	glTranslatef(-3.0f, 0.0f, 3.0f);
	LoadModel(teapot);

	glutPostRedisplay();
	glutSwapBuffers();

	angle_cube += 3;
	angle_elephant += 3;
	angle_whole += 1;
}

void reshape(GLsizei w, GLsizei h)
{
	windowSize[0] = w;
	windowSize[1] = h;
}

void LoadModel(Object* Model)
{
	for (size_t i = 0; i < Model->fNum; ++i)
	{
		glBegin(GL_TRIANGLES);
		for (size_t j = 0; j < 3; ++j)
		{
			glNormal3fv(Model->nList[Model->faceList[i][j].nIndex].ptr);
			glVertex3fv(Model->vList[Model->faceList[i][j].vIndex].ptr);
		}
		glEnd();
	}
}

void DrawBasis()
{
	///TO DO: draw basis
	///
	float x[] = { 5, 7, 9, 10, 10, 9, 7, 5, 2, -2, -5, -7, -9, -10, -10, -9, -7, -5, -2, 2 };
	float z[] = { -9, -7, -5, -2, 2, 5, 7, 9, 10, 10, 9, 7, 5, 2, -2, -5, -7, -9, -10, -10 };

	// sides
	/*
	x = 0
	y = 1
	for i in range(20):
		newx = cos(pi/10)*x - sin(pi/10)*y
		newy = sin(pi/10)*x + cos(pi/10)*y
		print ([newx, newy])
		x = newx
		y = newy
	*/
	float normalx[] = { 0.588, 0.809,  0.951,1,0.951,0.809,0.588,0.309,0,-0.309,-0.588,-0.809,-0.951,-1,-0.951,-0.809,-0.587,-0.309,0,0.309 };
	float normalz[] = { -0.809,-0.588,-0.309,0,0.309,0.588,0.809,0.951,1,0.951 , 0.809, 0.588, 0.309, 0,-0.309,-0.588,-0.809,-0.951,-1,-0.951 };

	glBegin(GL_QUADS);
	for (int i = 0; i < 20; i++) {
		glNormal3f(normalx[i], 0.0f, normalz[i]);
		int j = (i + 1) % 20;
		glVertex3f(x[i], 0.0f, z[i]);
		glVertex3f(x[j], 0.0f, z[j]);
		glVertex3f(x[j], -5.0f, z[j]);
		glVertex3f(x[i], -5.0f, z[i]);
	}
	glEnd();

	// bottom
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0f, 0.0f);
	for (int i = 0; i < 20; i++)
	{
		glVertex3f(x[i], -5.0f, z[i]);
	}
	glEnd();

	// top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < 20; i++)
	{
		glVertex3f(x[i], 0.0f, z[i]);
	}
	glEnd();
}

void keyboard(unsigned char key, int x, int y)
{
	/// TO DO: Set the keyboard control
	///
	switch (key)
	{
	case 'w':
		angle_camera_verticle++;
		break;
	case 's':
		angle_camera_verticle--;
		break;
	case 'd':
		angle_camera_horizontal++;
		break;
	case 'a':
		angle_camera_horizontal--;
		break;
	}
}