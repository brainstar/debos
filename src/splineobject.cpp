#include "splineobject.h"
using namespace std;

void SplineObject::draw() {
	for (list<Spline>::iterator it = splines.begin(); it != splines.end(); it++)
		it->draw();
}

void SplineObject::addSpline(float *a, float *k1, float *k2, float *b) {
	Spline spline(a, k1, k2, b);
	splines.push_back(spline);
	for (list<Spline>::iterator it = splines.begin(); it != splines.end(); it++)
		it->setColor(1.0, 1.0, 1.0);
	if (iter != splines.end())
		iter->setColor(1.0, 1.0, 1.0);
	iter = splines.end();
	iter--;
	iter->setColor(1.0, 0.0, 1.0);
}

void SplineObject::deleteSpline() {
	if (iter != splines.end()) {
		if ((iter = splines.erase(iter)) == splines.end())
			if (iter != splines.begin()) {
				iter--;
				iter->setColor(1.0, 0.0, 1.0);
			}
	}
}

void SplineObject::nextSpline() {
	if (iter != splines.end()) {
		iter->setColor(1.0, 1.0, 1.0);
		if (++iter == splines.end()) iter--;
		iter->setColor(1.0, 0.0, 1.0);
	}
}

void SplineObject::prevSpline() {
	if (iter != splines.end()) {
		iter->setColor(1.0, 1.0, 1.0);
		if (iter != splines.begin()) iter--;
		iter->setColor(1.0, 0.0, 1.0);
	}
}
