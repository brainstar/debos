#include "splineobject.h"
#include "Qt/qwidget.h" 
#include "QtOpenGL/qgl.h"
using namespace std;

SplineObject::SplineObject() {
	// Standard constructor
	for (int i = 0; i < 3; i++) {
		points[0][i] = points[1][i] = points[2][i] = points[3][i] = 0.0;
	}
	pCount = 0;
	iter = splines.end();
}

void SplineObject::draw() {
	// Draw the SplineObject
	for (list<Spline>::iterator it = splines.begin(); it != splines.end(); it++)
		it->draw();

/*	glColor3f(0.0f, 1.0f, 1.0f);
	glPointSize(5.0);
	glBegin(GL_POINTS);
		for (int i = 0; i < pCount; i++)
			glVertex3f(points[i][0], points[i][1], points[i][2]);
	glEnd(); */
}

void SplineObject::addSpline(float *a, float *k1, float *k2, float *b) {
	// Add Spline to the SplineObject
	Spline spline(a, k1, k2, b);
	splines.push_back(spline);
	for (list<Spline>::iterator it = splines.begin(); it != splines.end(); it++)
		it->setColor(1.0, 1.0, 1.0);
	iter = splines.end();
	iter--;
	iter->setColor(1.0, 0.0, 1.0);

	for (int i = 0; i < 3; i++) {
		points[0][i] = *(b + i);
		points[1][i] = *(b + i)*2 - *(k2 + i);
	}
	pCount = 2;
}

void SplineObject::deleteSpline() {
	// Delete selected Spline
	list<Spline>::iterator it = splines.end();
	if (it == splines.begin()) return;
	it--;
	if (iter == it || iter == splines.begin()) {
		iter = splines.erase(iter);
		if (iter == splines.end() && iter != splines.begin()) {
			iter--;
			iter->setColor(1.0, 0.0, 1.0);
		}
		qDebug("removed Spline");
	}
}

bool SplineObject::addPoint(float x, float y, float z) {
	// Add Point
	bool ret = false;

	if (pCount > 3) {
		addSpline(&points[0][0], &points[1][0], &points[2][0], &points[3][0]);
		ret = true;
	}

	points[pCount][0] = x;
	points[pCount][1] = y;
	points[pCount][2] = z;
	pCount++;

	if (pCount > 3) {
		addSpline(&points[0][0], &points[1][0], &points[2][0], &points[3][0]);
		ret = true;
	}

	return ret;
}

void SplineObject::nextSpline() {
	// Select next Spline
	if (iter != splines.end()) {
		iter->setColor(1.0, 1.0, 1.0);
		iter++;
		if (iter == splines.end()) iter--;
		iter->setColor(1.0, 0.0, 1.0);
	}
}

void SplineObject::prevSpline() {
	// Select previous Spline
	if (iter != splines.end()) {
		iter->setColor(1.0, 1.0, 1.0);
		if (iter != splines.begin()) {
			iter--;
		}
		iter->setColor(1.0, 0.0, 1.0);
	}
}
