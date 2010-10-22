/*
 * gl_drawer.cpp
 *
 *  Created on: 22.08.2010
 *      Author: brainstar
 */

#include "gl_drawer.h"
using namespace std;

GLDrawer::GLDrawer(QWidget *parent, char *name) : QGLWidget(parent) {
	setMouseTracking(true);
}

GLDrawer::~GLDrawer() {
}

void GLDrawer::ptc(int x, int y, float *fx, float *fy) {
	float w = (float) size().width();
	float h = (float) size().height();
	*fx = ((float) x / w) * (display[1] - display[0]) + display[0];
	*fy = (1. - (float) y / h) * (display[3] - display[2]) + display[2];
}

void GLDrawer::initializeGL() {
	// Initializing OpenGL
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_MAP1_VERTEX_3);
}

void GLDrawer::resizeGL(int w, int h) {
	// Resize the Window to display at least what is determined by the grid
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

	if (w > h) {
		display[0] = *g * (GLfloat) w / (GLfloat) h;
		display[1] = *(g+1) * (GLfloat) w / (GLfloat) h;
		display[2] = *(g+2);
		display[3] = *(g+3);
	}
	else {
		display[0] = *g;
		display[1] = *(g+1);
		display[2] = *(g+2) * (GLfloat) h / (GLfloat) w;
		display[3] = *(g+3) * (GLfloat) h / (GLfloat) w;
	}
	glOrtho(display[0], display[1], display[2], display[3], 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLDrawer::paintGL() {
	// Draw everything
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (data) {
		// Draw the grid
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_LINE);
			glVertex3f(display[0], 0.0, 0.0);
			glVertex3f(display[1], 0.0, 0.0);
			glVertex3f(0.0, display[2], 0.0);
			glVertex3f(0.0, display[3], 0.0);
		glEnd();
		// Draw the rest
		emit draw();
	}
}

void GLDrawer::simResize() {
	// Simulate a resize
	makeCurrent();
	resizeGL(size().width(), size().height());
}

QImage GLDrawer::getScreen() {
	// Return a screenshot
	return grabFrameBuffer ( false ); // false = no alpha
}

void GLDrawer::mousePressEvent(QMouseEvent * event) {
	// Event processing of a mouse click
	float x, y;
	x = (float)event->x() / size().width()  *  (display[1] - display[0]) + display[0];
	y = (size().height() - (float) event->y()) / size().height() * (display[3] - display[2]) + display[2];
	emit mouseClicked(x, y);

	QGLWidget::mousePressEvent(event);
}

void GLDrawer::mouseMoveEvent(QMouseEvent *event) {
	emit mouseMoved(event->globalX(), event->globalY());
}
