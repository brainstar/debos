#ifndef SPLINEOBJECT_H
#define SPLINEOBJECT_H

#include <list>
#include "spline.h"
#include "object.h"
#include "bezier.h"
#include <Qt/qwidget.h>
using namespace std;

class SplineObject : public Object {
public:
	SplineObject();
	~SplineObject() { }

	void draw(bool edit);

	bool addPoint(float x, float y, float z);
	
	void nextInstance();
	void prevInstance();
	void deleteInstance();
	
	void addBezierPoint(float *a, float *l, float *r);
	void turnBezierPoint(float x);
	void moveBezierPoint(float x, float y);
	void computeSplines();

	list<Spline> splines;
	list<BezierPoint> beziers;
private:
	list<BezierPoint>::iterator activeBezier;
};

#endif
