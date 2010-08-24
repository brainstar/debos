#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "splineobject.h"
#include "lineobject.h"
#include "tinyxml/tinyxml.h"
#include <string>
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

	bool save(string filename);
	bool load(string filename);

	void setGrid(float* bound);
	float *getGrid() { return bounds; }
	void setAuthor(string aut);
	void setDescription(string des);
private:
	list<SplineObject> spline_objects;
	list<SplineObject>::iterator so_iter;
	list<LineObject> line_objects;
	list<LineObject>::iterator l_iter;

	TiXmlDocument doc;
	float bounds[4];
	string author, description;
};

#endif
