//g++ -std=c++11 main.cpp  -lglut -lGL -lGLU  -o grilla.out

#include <iostream>
#include <fstream>
#include <sstream>
#define GLUT_DISABLE_ATEXIT_HACK	
#include <math.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <GL/glut.h>
#include <time.h>
#include "celda.h"
#include "common.hpp"
using namespace std;

//color del fondo
#define RED 0.57
#define GREEN 0.92
#define BLUE 0.94
#define ALPHA 1 

#define ECHAP 27

#define HEAD_INPUT_FILE "presentacion"
#define HEAD_DELAY_INPUT_FILE "local_planning"

#define EXT_INPUT_FILE ".csv"

#define GRID_REFERENCE 0
#define GRILLA_REF_X 20
#define GRILLA_REF_Y 20
#define GRILLA_REF_Z 1

#define GIZMO_REFERENCE 1
#define GIZMO_SIZE SIZE_EDGE

#define ANIMATED_VERSION 0
#define REF_PATHS 0
#define TIME_INTERVAL 0.035
#define START_REFERENCE 0
#define END_REFERENCE 0

GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);


float delta_x = 0.0;//-50.0; 
float delta_y = 180.0;//5.0;
float mouse_x, mouse_y;
float var_x = -10.0;//0.0;
float var_z = -15.0;
float var_y = 7.5;//0.0;
float step = 0; 

//0.0 Posicion inicial. 1.0 Traslacion. 2.0 Primera Rotacion(en y).
// 3.0 Segunda Rotacion (en x) 4.0 Ultima Rotacion (en z)

///////////////////////////////////////////////////////////////////////////////
//(1)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		var_z += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		var_z -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		var_x += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		var_x -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_F1:
		//step++;
		var_y -= 0.5;
		glutPostRedisplay();
		break;
	case GLUT_KEY_F2:
		//step--;
		var_y += 0.5;
		glutPostRedisplay();
		break;


	}
}
///////////////////////////////////////////////////////////////////////////////
//(2)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		mouse_x = x;
		mouse_y = y;
	}
}
///////////////////////////////////////////////////////////////////////////////
//(3)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_motion(int x, int y)
{
	
	delta_x += x - mouse_x;
	delta_y += y - mouse_y;
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}


GLvoid window_idle();

GLvoid initGL()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glClearColor(RED, GREEN, BLUE, ALPHA);
	//glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
}

Grilla grilla;
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Interfaz grilla 3D");

	initGL();
 
 	if(OBSTACLES){
 		grilla.loadPathInfo(OBSTACLES_FILE_NAME,0);	
 	}	
	
    grilla.loadPathInfoSet(NUMBER_OF_PATHS, HEAD_INPUT_FILE ,EXT_INPUT_FILE);
    if(ANIMATED_VERSION){
    	grilla.loadPathInfoRef();
    	cout<<"MAX_PATH_SIZE: "<<grilla.MAX_PATH_SIZE<<"MAX_PATH_ID: "<<grilla.MAX_PATH_ID<<endl;
    	grilla.loadDelays(NUMBER_OF_PATHS,HEAD_DELAY_INPUT_FILE,EXT_INPUT_FILE);
    	cout<<"currentPos.size() "<<grilla.currentPos.size()<<endl;
    }
	
	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);
		
	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);

	glutKeyboardFunc(&window_key);
	glutSpecialFunc(&callback_special);
	
	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();
	return 1;
}

void Gizmo3D(){
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);///ROJO X
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(GIZMO_SIZE, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f); ///VERDE Y
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, GIZMO_SIZE, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);///AZUL Z
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, GIZMO_SIZE);
	glEnd();
	return;
}

void delay(float secs)
{
	float end = clock()/CLOCKS_PER_SEC + secs;
	while((clock()/CLOCKS_PER_SEC) < end);
}

double dt, currentTime, lastTime = 0.0;
float t = 0.0f;

GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(60,800.0/600.0, 0.1, 10000);

	glTranslatef(var_x, var_y, var_z);
	glRotatef(delta_x, 0.0, 1.0, 0.0);
	glRotatef(delta_y, 1.0, 0.0, 0.0);

	if(GIZMO_REFERENCE){
		Gizmo3D();
	}

	if(START_REFERENCE){
		for(int i = 0 ; i< grilla.startPath.size();++i){
			grilla.startPath[i].drawCel();
		}
	}

	if(END_REFERENCE){
		for(int i = 0 ; i< grilla.endPath.size();++i){
			grilla.endPath[i].drawCel();
		}
	}

	if(GRID_REFERENCE){
		grilla.drawGrillaRef(GRILLA_REF_X,GRILLA_REF_Y,GRILLA_REF_Z);	
	}
	
	if (ANIMATED_VERSION){
		
		if( (t/1.0) > grilla.MAX_PATH_SIZE){
			t = 0.0;
			for(int i = 0 ; i< grilla.currentPos.size();++i){
				grilla.currentPos[i]=0;
			}
			grilla.Delays = grilla.DelaysOriginals;
		}
		grilla.drawAllAnimatedPath(t);
		t += TIME_INTERVAL;
		//cout<<"t: "<<t<<endl;	
		if (REF_PATHS){
			grilla.drawAllPaths(false);
		}
	}
	else{
		grilla.drawAllPaths();
	}
	
	glutSwapBuffers();
	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key) {
	case ECHAP:
		exit(1);
		break;
	default:
		printf("La tecla %d no tiene significado.\n", key);
		break;
	}
}

GLvoid window_idle()
{
	glutPostRedisplay();
}




