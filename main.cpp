#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include "custtran.h"

using namespace std;

#define PI 3.14159

//dynamic rotating angles for camera 
float cam_rotx = 0, cam_roty = -45, cam_rotz = 0;

//dynamic rotating angle for car 
float car_rot = 0;

//movement controls for car 
float car_movx = 0, car_movz = 0;

//variable to toggle camera view
int view = 0;

//variable to change color headlights
int headlights = 0;

//variable to switch between day mode and night node
int md = 0;

//variables of tree coordinates
int trx = 0, trz = -10;

//variables for controlling zoom
float zoom = 0, zfac;

//variable to control sun/moon movement
float sun = 10, sunrate = 0.1;

// function to draw wheel
void drawWheel(float x, float y, float z)
{	
	glPushMatrix();

	glBegin(GL_POLYGON);
	glColor3f(0,0,0);

	for(float i = 0; i < 2 * PI; i += PI / 36)
	{
 		glVertex3f(x + cos(i), y + sin(i), z);
	}

	glEnd();

	glPopMatrix();
}

//function to draw car
void drawCar()
{	
	glPushMatrix();

	translate(car_movx, car_movz);
	rotate(car_rot);

	//lower part of car
	glPushMatrix();
	glScalef(3,1,1);
	glLineWidth(2);

	glColor3f(0,0,0);
	glutWireCube(3);
	glColor3f(1,0,0);
	glutSolidCube(3);

	glPopMatrix();

	//----------END

	//upper part of car
	glPushMatrix();
	glTranslatef(-1.5,3,0);
	shear(0.5,0);
	glLineWidth(5);

	glColor3f(0,0,1);
	glutWireCube(3);
	glColor3f(1,1,1);
	glutSolidCube(3);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5,3,0);
	shear(-0.5,0);

	glColor3f(0,0,1);
	glutWireCube(3);
	glColor3f(1,1,1);
	glutSolidCube(3);

	glPopMatrix();
	
	//adding an extra cube on top
	glTranslatef(0,3,0);
	glutSolidCube(3);
	
	//----------END

	//wheels
	for (int i = 0; i < 40; ++i)
	{
		float f = i * 0.01;

		drawWheel(3,-5,1.6+f);
		drawWheel(-3,-5,1.6+f);
		drawWheel(3,-5,-1.6-f);
		drawWheel(-3,-5,-1.6-f);
	}

 	glLineWidth(1);
	glPopMatrix();
}

//function to draw tree and set up the environment
void drawTree(int x, int z)
{
	//trunk of the tree
	glPushMatrix();
	
	translate(x,z);
	glScalef(1,3,1);
	glColor3f(0.75,0.2545,0.2545);
	glutSolidCube(2);
	
	glPopMatrix();

	//upper green part of the tree
	glPushMatrix();
	
	translate(x,z);
	glRotated(90, -1.0, 0.0, 0.0);
	glColor3f(0.2456,0.75,0.2456);
	glutSolidCone(4,10,80,80);

	glPopMatrix();
}

//function to draw the testing grid (X-Z Plane)
void drawGrid()
{ 
	for (int i = 0; i < 50; i += 5)
	{
		glPushMatrix();

		glBegin(GL_LINES);
		
		//to draw lines parallel to z-axis
		glVertex3f(i,0,0);
		glVertex3f(i,0,50);

		//to draw lines parallel to x-axis
		glVertex3f(0,0,i);
		glVertex3f(50,0,i);

		glEnd();

		glPopMatrix();
	}	
}

//function to draw the base plane (X-Z Plane)
void drawPlane()
{
	glPushMatrix();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 	
 	glBegin(GL_QUADS);
 	glColor3f(1,0.8,0);
 	glVertex3f(-2500.0f, -4.0f, 0.0f);
 	glVertex3f( 0.0f, -4.0f, 2500.0f);
 	glVertex3f( 2500.0f, -4.0f, 0.0f);
	glVertex3f( 0.0f, -4.0f, -2500.0f);
	glEnd();

	glPopMatrix();
}

void lighting()
{
	float theta = sun * (PI/180);
	float srcx = 200 * cos(theta);
	float srcy = 200 * sin(theta);

	glPushMatrix();

	//sun
	if (md == 0)
	{
		glColor3f(1,1,1);
		glTranslatef(srcx, srcy, 0);
		glutSolidCube(5);
		glTranslatef(-srcx, -srcy, 0);

		//adding ambient, diffuse and specular models
		GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0}; 
		GLfloat light_diffuse[] = {0.8, 0.8, 0.8, 1.0}; 
		GLfloat light_specular[] = {0.5, 0.5, 0.5, 1.0}; 
		GLfloat light_position[] = {srcx/2, srcy/2, 0.0, 1.0};

		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); 
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); 
		glLightfv(GL_LIGHT0, GL_POSITION, light_position); 

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
	}

	//moon
	else
	{
		glColor3f(1,1,0);
		glTranslatef(srcx, srcy, 0);
		glutSolidCube(5);
		glTranslatef(-srcx, -srcy, 0);

		//adding ambient, diffuse and specular models
		GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0}; 
		GLfloat light_diffuse[] = {0.9, 0.9, 0.9, 1.0}; 
		GLfloat light_specular[] = {0.3, 0.3, 0.3, 1.0}; 
		GLfloat light_position[] = {srcx/2, srcy/2, 0.0, 1.0}; 	

		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); 
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); 
		glLightfv(GL_LIGHT0, GL_POSITION, light_position); 

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
	}

	glPopMatrix();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//zooming restriction
	zfac = -50-zoom;
	if(zfac >= 0) 
	{
		zfac = 0;
		zoom = -50;
	}	

	if(view == 0)
	{
		//setting up camera
		glTranslatef(0,0,zfac);
		glRotatef(cam_rotx,1,0,0);
		glRotatef(cam_roty,0,1,0);
		glRotatef(cam_rotz,0,0,1);
	}

	else
	{
		//setting up camera
		glTranslatef(0,0,zfac);
		glRotatef(cam_rotx,1,0,0);
		glRotatef(cam_roty - 45,0,1,0);
		glRotatef(cam_rotz,0,0,1);

		glRotatef(car_rot,0,1,0);
		glTranslatef(-car_movx, 0, -car_movz);
	}

	lighting();
	drawPlane();
	
	//trx = rand()*10;
	//trz = rand()*10;
	drawCar();
	drawTree(trx,trz);
	//drawGrid();
	glutSwapBuffers();
}

void KeyboardInput(unsigned char key, int x, int y) 
{  
	switch(key)
	{
		case 'x':	if(cam_rotx <= 175)
						cam_rotx += 5;
					break;

		case 's':	if(cam_rotx >= 5)
						cam_rotx -= 5;
					break;
		
		case 'y':	cam_roty += 5;
					break;

		case 'z':	if(cam_rotz < -5)
						cam_rotz += 5;
					break;

		case 'a':	if(cam_rotz >= -45)
						cam_rotz -= 5;
					break;

		case 'v': 	view = (view + 1) % 2;
					md = (md + 1) % 2;
					break;

		case 'c': 	zoom -=  10;
					break;

		case 'd': 	zoom +=  10;
					break;	

		case 'l': 	sunrate += 0.1;
					break;

		case 'k':	if(sunrate > 0.1)
						sunrate -= 0.1;
					break;									
	}
	glutPostRedisplay();
}  

void SpecialInput(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP: 		car_movx -= cos(car_rot * (PI/180));
								car_movz -= sin(car_rot * (PI/180));
								break;

		case GLUT_KEY_DOWN: 	car_movx += cos(car_rot * (PI/180));
								car_movz += sin(car_rot * (PI/180));
								break;

		case GLUT_KEY_RIGHT:  	car_rot += 10;
								break;

		case GLUT_KEY_LEFT: 	car_rot -= 10;
								break;
	}
	glutPostRedisplay();
}

void MouseInput(int key, int state, int x, int y)
{
	//headlights on/off
	//if(key == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		
	//switch between day mode and night mode
	if(key == GLUT_LEFT_BUTTON && state == GLUT_UP)
		md = (md + 1)%2;
	

	glutPostRedisplay();
}

void init()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//to fix the camera clipping
	gluPerspective(60,1.0f,0.1f,1000);
	
	glMatrixMode(GL_MODELVIEW);
	
	//to add proper depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 

	glEnable(GL_LINE_SMOOTH);

 	glClearColor(0.5,0.5,1,1);
}

//timer function to control day time
void timer(int t)
{
	float theta = sun * (PI/180);
	glutTimerFunc(10, timer, 0);

	sun += sunrate;

	if(sun > 180)
		sun = 0;
	
	glutPostRedisplay();

	if(md == 0)
		glClearColor(abs(cos(theta)),0,abs(sin(theta)),0.5);
	else 
		glClearColor(abs(0.2*cos(theta)), abs(0.2*cos(theta)), abs(0.2*cos(theta)),0.5);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	 	
	glutInitWindowSize(1000,800);
	glutCreateWindow("CG Project");
	 	
	glutDisplayFunc(display);
	glutSpecialFunc(SpecialInput);
	glutKeyboardFunc(KeyboardInput);
	glutMouseFunc(MouseInput);
	init();
	glutTimerFunc(10, timer, 0);
	glutMainLoop();

 	return 0;
}
