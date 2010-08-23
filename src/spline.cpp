#include "spline.h"
#include <QtOpenGL/qgl.h>

Spline::Spline() {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
			points[i][j] = 0;
}

Spline::Spline(float *a, float *k1, float *k2, float *b) {
	for (int i = 0; i < 3; i++) {
		points[0][i] = *(a + i);
		points[1][i] = *(k1 + i);
		points[2][i] = *(k2 + i);
		points[3][i] = *(b + i);
	}
}


void Spline::seta(float *p) {
	points[0][0] = *p;
	points[0][1] = *(p + 1);
	points[0][2] = *(p + 2);
}

void Spline::setk1(float *p) {
        points[1][0] = *p;
        points[1][1] = *(p + 1);
        points[1][2] = *(p + 2);
}

void Spline::setk2(float *p) {
        points[2][0] = *p;
        points[2][1] = *(p + 1);
        points[2][2] = *(p + 2);
}

void Spline::setb(float *p) {
        points[3][0] = *p;
        points[3][1] = *(p + 1);
        points[3][2] = *(p + 2);
}

void Spline::draw() {
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &points[0][0]);
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= 30; i++)
			glEvalCoord1f( (GLfloat) i / 30.0);
	glEnd();
}
