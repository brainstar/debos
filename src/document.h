#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "splineobject.h"
using namespace std;

class Document {
public:
	Document() { so_iter = spline_objects.end(); }

	void draw();
	
	void addSplineObject();
	void deleteSplineObject();

	void nextSplineObject();
	void prevSplineObject();
	SplineObject* getSplineObject();

private:
	list<SplineObject> spline_objects;
	list<SplineObject>::iterator so_iter;
};

#endif
