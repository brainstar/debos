#include "splineobject.h"
#include "Qt/qwidget.h" 
#include "QtOpenGL/qgl.h"
using namespace std;

SplineObject::SplineObject() {
	// Standard constructor
	activeBezier = beziers.end();
	type = SPLINE;
	bClosed = false;
}

void SplineObject::nextInstance() {
	if (activeBezier == beziers.end()) return;
	activeBezier++;
	if (activeBezier == beziers.end()) {
		if (bClosed) {
			activeBezier = beziers.begin();
		}
		else {
			activeBezier--;
		}
	}
}

void SplineObject::prevInstance() {
	if (activeBezier == beziers.begin()) {
		if (bClosed) {
			activeBezier = beziers.end();
		}
		else {
			return;
		}
	}
	activeBezier--;
}

void SplineObject::deleteInstance() {
	if (activeBezier == beziers.end()) return;
	activeBezier = beziers.erase(activeBezier);
	if (activeBezier != beziers.begin()) activeBezier--;
	
	computeSplines();
}

void SplineObject::draw(bool edit) {
	// Draw the SplineObject
	list<Spline>::iterator it;
	for (it = splines.begin(); it != splines.end(); it++)
		it->draw(edit);
	it--;

	// Draw the active BezierPoint
	if (edit) {
		if (activeBezier != beziers.end()) {
			float *a[3];
			a[0] = activeBezier->getpl();
			a[1] = activeBezier->getp();
			a[2] = activeBezier->getpr();
			glColor3f(0.0f, 1.0f, 1.0f);
			glPointSize(5.0);
			glBegin(GL_POINTS);
				for (int i = 0; i < 3; i++)
					glVertex3f(a[i][0], a[i][1], a[i][2]);
			glEnd();
		}
	}
}

bool SplineObject::addPoint(float x, float y, float z) {
	// Add Point
	bool ret = false;
	
	float p[3], a[3], b[3];
	
	p[0] = a[0] = b[0] = x;
	p[1] = a[1] = b[1] = y;
	p[2] = a[2] = b[2] = z;
	
	a[0] -= 1.;
	b[0] += 1.;
	
	addBezierPoint(p, a, b);

	return ret;
}

void SplineObject::addBezierPoint(float *a, float *l, float *r) {
	BezierPoint p(a, l, r);
	beziers.push_back(p);

	activeBezier = beziers.end();
	activeBezier--;

	computeSplines();
}

void SplineObject::turnBezierPoint(float x) {
	if (activeBezier != beziers.end())
		activeBezier->turn(x);
	computeSplines();
}

void SplineObject::turnBezierPoint(float FromX, float FromY, float ToX, float ToY) {
	if (activeBezier != beziers.end()) {
		float a = activeBezier->getAngle(FromX, FromY);
		float b = activeBezier->getAngle(ToX, ToY);
		float c = b - a;
		activeBezier->turn(c);
	}
	
	computeSplines();
}

void SplineObject::moveBezierPoint(float x, float y) {
	if (activeBezier != beziers.end())
		activeBezier->move(x, y);
	
	computeSplines();
}

void SplineObject::saveBezierPoint() {
	if (activeBezier != beziers.end()) {
		activeBezier->save();
	}
}

void SplineObject::recoverBezierPoint() {
	if (activeBezier != beziers.end()) {
		activeBezier->recover();
	}
	
	computeSplines();
}

void SplineObject::scaleBezierPoint(float FromX, float FromY, float ToX, float ToY) {
	if (activeBezier != beziers.end()) {
		activeBezier->scale(activeBezier->getLength(ToX, ToY) / activeBezier->getLength(FromX, FromY));
	}
	
	computeSplines();
}

void SplineObject::computeSplines() {
	list<Spline>::iterator it = splines.begin();
	while (it != splines.end()) it = splines.erase(it);
	
	list<BezierPoint>::iterator it1 = beziers.begin(), it2 = beziers.begin();
	it2++;
	while (it2 != beziers.end()) {
		Spline a(it1->getp(), it1->getpr(), it2->getpl(), it2->getp());
		a.setActive(false);
		splines.push_back(a);
		it1++;
		it2++;
	}
	
	if (bClosed && it1 != beziers.end()) {
		it2 = beziers.begin();
		Spline a(it1->getp(), it1->getpr(), it2->getpl(), it2->getp());
		a.setActive(false);
		splines.push_back(a);
	}
	
	it = splines.end();
	it--;
}
