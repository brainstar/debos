#ifndef LINEOBJECT_H
#define LINEOBJECT_H

#include <list>
#include "line.h"
using namespace std;

class LineObject {
public:
	LineObject();
	~LineObject() {  }

	void draw();

	void addLine(float *a, float *b);
	void deleteLine();

	bool addPoint(float x, float y, float z);

	void nextLine();
	void prevLine();

	void iterToEnd() { iter = lines.end(); }

	list<Line> lines;

private:
	float points[2][3];
	int pCount;
	list<Line>::iterator iter;
};

#endif
