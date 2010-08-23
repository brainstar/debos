/*
 * gl_drawer.cpp
 *
 *  Created on: 22.08.2010
 *      Author: brainstar
 */

#include "gl_drawer.h"

void GLDrawer::initializeGL() {
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_MAP1_VERTEX_3);

	float a[3] = { -4.0, -4.0, 0.0 };
	float b[3] = { 2.0, 4.0, 0.0 };
	float c[3] = { -2.0, -4.0, 0.0 };
	float d[3] = { 4.0, 4.0, 0.0 };

	spline = new Spline(a, b, c, d);
}

void GLDrawer::resizeGL(int w, int h) {
	h = h ? h : 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat) w / (GLfloat) h, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLDrawer::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -12.0f);

	spline->draw();
}
