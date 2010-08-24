#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "splineobject.h"
#include "lineobject.h"
using namespace std;

class Document {
public:
	Document() { so_iter = spline_objects.end(); }

	void draw();
	
	void addSplineObject();
	void deleteSplineObject();
	void addLineObject();
	void deleteLineObject();

	void nextSplineObject();
	void prevSplineObject();
	SplineObject* getSplineObject();
	void nextLineObject();
	void prevLineObject();
	LineObject* getLineObject();

private:
	list<SplineObject> spline_objects;
	list<SplineObject>::iterator so_iter;
	list<LineObject> line_objects;
	list<LineObject>::iterator l_iter;
};

#endif
