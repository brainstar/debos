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
	void turnBezierPoint(float FromX, float FromY, float ToX, float ToY);
	void moveBezierPoint(float x, float y);
	void scaleBezierPoint(float FromX, float FromY, float ToX, float ToY);
	void computeSplines();

	list<Spline> splines;
	list<BezierPoint> beziers;
	
	bool bClosed;
	
private:
	list<BezierPoint>::iterator activeBezier;
};

#endif
