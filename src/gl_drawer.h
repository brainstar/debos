/*
 * gl_drawer.h
 *
 *  Created on: 22.08.2010
 *      Author: brainstar
 */

#ifndef GL_DRAWER_H
#define GL_DRAWER_H

#include <QtOpenGL/qgl.h>
using namespace std;

class GLDrawer : public QGLWidget {
Q_OBJECT

public:
	GLDrawer (QWidget *parent = 0, char *name = 0) : QGLWidget(parent) {}

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
};

#endif
