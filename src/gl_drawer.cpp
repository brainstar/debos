/*
 * gl_drawer.cpp
 *
 *  Created on: 22.08.2010
 *      Author: brainstar
 */

#include "gl_drawer.h"
using namespace std;

GLDrawer::GLDrawer(QWidget *parent, char *name) : QGLWidget(parent) {
}

GLDrawer::~GLDrawer() {
}

void GLDrawer::initializeGL() {
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_MAP1_VERTEX_3);
}

void GLDrawer::resizeGL(int w, int h) {
	h = h ? h : 1;
	w = w ? w : 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	float foo[4] = {-10.0, 10.0, -10.0, 10.0 };
	float *g = foo;
	if (data) {
		g = data->getGrid();
	}

	if (w > h)
		glOrtho(*g * (GLfloat) w / (GLfloat) h, *(g + 1) * (GLfloat) w / (GLfloat) h, *(g + 2), *(g + 3), 1.0, -1.0);
	else
		glOrtho(*g, *(g + 1), *(g + 2) * (GLfloat) h / (GLfloat) w, *(g + 3) * (GLfloat) h / (GLfloat) w, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLDrawer::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (data)
		data->draw();
}

QImage GLDrawer::getScreen() {
	return grabFrameBuffer ( false ); // false = no alpha
}
