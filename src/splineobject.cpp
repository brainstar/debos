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
	activeSpline = splines.end();
	type = SPLINE;
}

void SplineObject::draw(bool edit) {
	// Draw the SplineObject
	list<Spline>::iterator it;
	for (it = splines.begin(); it != splines.end(); it++)
		it->draw(edit);
	it--;

	if (activeSpline == it && edit) {
		glColor3f(0.0f, 1.0f, 1.0f);
		glPointSize(5.0);
		glBegin(GL_POINTS);
			for (int i = 0; i < pCount; i++)
				glVertex3f(points[i][0], points[i][1], points[i][2]);
		glEnd();
	}
}

void SplineObject::addInstance(float *a, float *k1, float *k2, float *b) {
	// Add Spline to the SplineObject
	Spline spline(a, k1, k2, b);
	splines.push_back(spline);
	for (list<Spline>::iterator it = splines.begin(); it != splines.end(); it++)
		it->setActive(false);
	activeSpline = splines.end();
	activeSpline--;
	activeSpline->setActive(true);

	for (int i = 0; i < 3; i++) {
		points[0][i] = *(b + i);
		points[1][i] = *(b + i)*2 - *(k2 + i);
	}
	pCount = 2;
}

void SplineObject::addInstance(float *a, float *b) { }

void SplineObject::deleteInstance() {
	// Delete selected Spline
	list<Spline>::iterator it = splines.end();
	if (it == splines.begin()) return;
	it--;
	if (activeSpline == splines.begin()) {
		activeSpline = splines.erase(activeSpline);
	}
	else if (activeSpline == it) {
		activeSpline == splines.erase(activeSpline);
		activeSpline--;
	}
	if (activeSpline != splines.end()) {
		activeSpline->setActive(true);
	}
	qDebug("removed Spline");
}

bool SplineObject::addPoint(float x, float y, float z) {
	// Add Point
	bool ret = false;

	if (pCount > 3) {
		addInstance(&points[0][0], &points[1][0], &points[2][0], &points[3][0]);
		ret = true;
	}

	points[pCount][0] = x;
	points[pCount][1] = y;
	points[pCount][2] = z;
	pCount++;

	if (pCount > 3) {
		addInstance(&points[0][0], &points[1][0], &points[2][0], &points[3][0]);
		ret = true;
	}

	return ret;
}

void SplineObject::nextInstance() {
	// Select next Spline
	if (activeSpline != splines.end()) {
		activeSpline->setActive(false);
		activeSpline++;
		if (activeSpline == splines.end()) activeSpline--;
		activeSpline->setActive(true);
	}
}

void SplineObject::prevInstance() {
	// Select previous Spline
	if (activeSpline != splines.end()) {
		activeSpline->setActive(false);
		if (activeSpline != splines.begin()) {
			activeSpline--;
		}
		activeSpline->setActive(true);
	}
}

void SplineObject::iterToEnd() {
	activeSpline = splines.end();
	activeSpline--;
}
