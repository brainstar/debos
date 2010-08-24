#ifndef SPLINEOBJECT_H
#define SPLINEOBJECT_H

#include <list>
#include "spline.h"
using namespace std;

class SplineObject {
public:
	SplineObject() { iter = splines.end(); }
	~SplineObject() { }

	void draw();

	void addSpline(float *a, float *k1, float *k2, float *b);
	void deleteSpline();

	void nextSpline();
	void prevSpline();

private:
	list<Spline> splines;
	list<Spline>::iterator iter;
};

#endif
