#include "splineobject.h"
#include "Qt/qwidget.h"
using namespace std;

SplineObject::SplineObject() {
	for (int i = 0; i < 3; i++) {
		points[0][i] = points[1][i] = points[2][i] = points[3][i] = 0.0;
	}
	pCount = 0;
	iter = splines.end();
}

void SplineObject::draw() {
	for (list<Spline>::iterator it = splines.begin(); it != splines.end(); it++)
		it->draw();
}

void SplineObject::addSpline(float *a, float *k1, float *k2, float *b) {
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
	list<Spline>::iterator it = splines.end();
	it--;
	if (iter == it || iter == splines.begin()) {
		iter = splines.erase(iter);
		if (iter == splines.end() && iter != splines.begin()) {
			iter--;
			iter->setColor(1.0, 0.0, 1.0);
		}
	}
	if (iter != splines.end()) {
		if ((iter = splines.erase(iter)) == splines.end())
			if (iter != splines.begin()) {
				iter--;
				iter->setColor(1.0, 0.0, 1.0);
			}
	}
}

void SplineObject::addPoint(float x, float y, float z) {
	if (pCount > 3) {
		addSpline(&points[0][0], &points[1][0], &points[2][0], &points[3][0]);
	}

	points[pCount][0] = x;
	points[pCount][1] = y;
	points[pCount][2] = z;
	pCount++;

	if (pCount > 3) {
		addSpline(&points[0][0], &points[1][0], &points[2][0], &points[3][0]);
	}
}

void SplineObject::nextSpline() {
	if (iter != splines.end()) {
		iter->setColor(1.0, 1.0, 1.0);
		if (++iter == splines.end()) iter--;
		else qDebug("next Spline");
		iter->setColor(1.0, 0.0, 1.0);
	}
}

void SplineObject::prevSpline() {
	if (iter != splines.end()) {
		iter->setColor(1.0, 1.0, 1.0);
		if (iter != splines.begin()) {
			iter--;
			qDebug("previous Spline");
		}
		iter->setColor(1.0, 0.0, 1.0);
	}
}
