#include "spline.h"
#include <QtOpenGL/qgl.h>
using namespace std;

Spline::Spline() {
	// Standrad constructor
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
			points[i][j] = 0;
	col[0] = col[1] = col[2] = 1.0f;
}

Spline::Spline(float *a, float *k1, float *k2, float *b) {
	// Constructor
	for (int i = 0; i < 3; i++) {
		points[0][i] = *(a + i);
		points[1][i] = *(k1 + i);
		points[2][i] = *(k2 + i);
		points[3][i] = *(b + i);
	}
	col[0] = col[1] = col[2] = 1.0f;
}


void Spline::seta(float *p) {
	// Set starting point
	points[0][0] = *p;
	points[0][1] = *(p + 1);
	points[0][2] = *(p + 2);
}

void Spline::setk1(float *p) {
	// Set control point 1
        points[1][0] = *p;
        points[1][1] = *(p + 1);
        points[1][2] = *(p + 2);
}

void Spline::setk2(float *p) {
	// Set control point 2
        points[2][0] = *p;
        points[2][1] = *(p + 1);
        points[2][2] = *(p + 2);
}

void Spline::setb(float *p) {
	// Set end point
        points[3][0] = *p;
        points[3][1] = *(p + 1);
        points[3][2] = *(p + 2);
}

void Spline::setColor(float r, float g, float b) {
	// Set color
	col[0] = r;
	col[1] = g;
	col[2] = b;
}

void Spline::draw() {
	// Draw the Spline
	glColor3f(col[0], col[1], col[2]);
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &points[0][0]);
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= 30; i++)
			glEvalCoord1f( (GLfloat) i / 30.0);
	glEnd();

	glPointSize(4.0);
	glBegin(GL_POINT);
		for (int i = 0; i < 4; i++)
			glVertex2fv(points[i]);
	glEnd();
}
