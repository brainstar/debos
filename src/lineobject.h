#ifndef LINEOBJECT_H
#define LINEOBJECT_H

#include <list>
#include "line.h"
#include "object.h"
using namespace std;

class LineObject : public Object {
public:
	LineObject();
	~LineObject() {  }

	void draw(bool edit);
	
	bool addPoint(float x, float y, float z);
	void nextInstance();
	void prevInstance();
	void addInstance(float *a, float *k1, float *k2, float *b);
	void addInstance(float *a, float *b);
	void deleteInstance();

	void iterToEnd();
	
	list<Line> lines;
private:
	list<Line>::iterator activeLine;
};

#endif
