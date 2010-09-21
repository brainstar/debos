#include "line.h"
#include <QtOpenGL/qgl.h>
using namespace std;

Line::Line() {
	// Standard constructor
	a[0] = a[1] = a[2] = 0.0;
	b[0] = b[1] = b[2] = 0.0;
	col[0] = col[1] = col[2] = 1.0;
}

Line::Line(float *x, float *y) {
	// Constructor
	a[0] = *x;
	a[1] = *(x + 1);
	a[2] = *(x + 2);
	b[0] = *y;
	b[1] = *(y + 1);
	b[2] = *(y + 2);
	col[0] = col[1] = col[2] = 1.0;
}

float* Line::geta() {
	// Return start point
	return a;
}

float* Line::getb() {
	// Return end point
	return b;
}

void Line::setColor(float r, float g, float b) {
	// Set the color
	col[0] = r;
	col[1] = g;
	col[2] = b;
}

void Line::draw(bool edit) {
	// Draw the line
	glColor3f(col[0], col[1], col[2]);

	glBegin(GL_LINE);
		glVertex3f(a[0], a[1], a[2]);
		glVertex3f(b[0], b[1], b[2]);
	glEnd();
}
