/*
 * gl_drawer.cpp
 *
 *  Created on: 22.08.2010
 *      Author: brainstar
 */

#include "gl_drawer.h"
using namespace std;

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

	if (w > h)
		glOrtho(*grid * (GLfloat) w / (GLfloat) h, *(grid + 1) * (GLfloat) w / (GLfloat) h, *(grid + 2), *(grid + 3), 1.0, -1.0);
	else
		glOrtho(*grid, *(grid + 1), *(grid + 2) * (GLfloat) h / (GLfloat) w, *(grid + 3) * (GLfloat) h / (GLfloat) w, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLDrawer::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	data->draw();
}
