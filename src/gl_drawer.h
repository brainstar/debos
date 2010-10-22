/*
 * gl_drawer.h
 *
 *  Created on: 22.08.2010
 *      Author: brainstar
 */

#ifndef GL_DRAWER_H
#define GL_DRAWER_H

#include <QtOpenGL/qgl.h>
#include <QImage>
#include <QMouseEvent>
#include "document.h"
using namespace std;

class GLDrawer : public QGLWidget {
Q_OBJECT

public:
	GLDrawer(QWidget *parent = 0, char *name = 0);
	~GLDrawer();
	QImage getScreen();
	Document* data;

	void simResize();
	void ptc(int x, int y, float *fx, float *fy);

signals:
	void mouseClicked( float x, float y );
	void mouseMoved(int x, int y);
	void draw();

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void mousePressEvent( QMouseEvent *event );
	void mouseMoveEvent(QMouseEvent *event);
	
private:
	float display[4]; // current size of the display, bigger than the size defined in data
};

#endif
