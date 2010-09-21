#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "splineobject.h"
#include "lineobject.h"
#include <string>
using namespace std;

class Document {
public:
	Document();

	void draw(bool edit);
	
	void addSplineObject();
	void addLineObject();
	void deleteObject();

	void nextObject();
	void prevObject();
	Object* getObject();

	bool save(string filename);
	bool load(string filename);

	void setGrid(float* bound);
	float *getGrid() { return bounds; }
	void setAuthor(string aut);
	void setDescription(string des);
private:
	list<Object*> objects;
	list<Object*>::iterator activeObject;

	float bounds[4];
	string author, description;
};

#endif
