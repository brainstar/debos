#ifndef SPLINEOBJECT_H
#define SPLINEOBJECT_H

#include <list>
#include "spline.h"
#include <Qt/qwidget.h>
using namespace std;

class SplineObject {
public:
	SplineObject();
	~SplineObject() { }

	void draw();

	void addSpline(float *a, float *k1, float *k2, float *b);
	void deleteSpline();

	void addPoint(float x, float y, float z);

	void nextSpline();
	void prevSpline();

	void iterToEnd() { iter = splines.end(); qDebug("SO iterToEnd: %i", &(*iter));}

	list<Spline> splines;

private:
	float points[4][3];
	int pCount;
	list<Spline>::iterator iter;
};

#endif
