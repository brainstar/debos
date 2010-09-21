#ifndef SPLINEOBJECT_H
#define SPLINEOBJECT_H

#include <list>
#include "spline.h"
#include "object.h"
#include <Qt/qwidget.h>
using namespace std;

class SplineObject : public Object {
public:
	SplineObject();
	~SplineObject() { }

	void draw();

	bool addPoint(float x, float y, float z);
	void nextInstance();
	void prevInstance();
	void addInstance(float *a, float *k1, float *k2, float *b);
	void addInstance(float *a, float *b);
	void deleteInstance();

	void iterToEnd();

	list<Spline> splines;
private:
	list<Spline>::iterator activeSpline;
};

#endif
