#ifndef LINEOBJECT_H
#define LINEOBJECT_H

#include <list>
#include "line.h"
using namespace std;

class LineObject {
public:
	LineObject() { iter = lines.end(); }
	~LineObject() {  }

	void draw();

	void addLine(float *a, float *b);
	void deleteLine();

	void nextLine();
	void prevLine();

private:
	list<Line> lines;
	list<Line>::iterator iter;
};

#endif
