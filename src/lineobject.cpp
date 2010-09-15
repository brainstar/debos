#include "lineobject.h"
#include <stdio.h>
using namespace std;

LineObject::LineObject() {
	// Standard constructor
	for (int i = 0; i < 3; i++) {
		points[0][i] = points[1][i] = 0.0;
	}
	pCount = 0;
	iter = lines.end();
}

void LineObject::draw() {
	// Draw the LineObject
	for (list<Line>::iterator it = lines.begin(); it != lines.end(); it++)
		it->draw();
}

void LineObject::addLine(float *a, float *b) {
	// Add a Line
	Line l(a, b);
	l.setColor(1.0, 0.0, 1.0);
	for(list<Line>::iterator it = lines.begin(); it != lines.end(); it++)
		it->setColor(1.0, 1.0, 1.0);
	lines.push_back(l);
	iter = lines.end();
	iter--;

	for (int i = 0; i < 3; i++) {
		points[0][i] = *(b + i);
	}
	pCount = 1;
}

void LineObject::deleteLine() {
	// Delete selected Line
	list<Line>::iterator it = lines.end();
	if (it == lines.begin()) return;
	it--;
	if (iter == it || iter == lines.begin()) {
		iter = lines.erase(iter);
		if (iter == lines.end() && iter != lines.begin()) {
			iter--;
			iter->setColor(1.0, 0.0, 1.0);
		}
	}
}

bool LineObject::addPoint(float x, float y, float z) {
	// Append point to the LineObject
	bool ret = false;

	if(pCount > 1) {
		addLine(&points[0][0], &points[1][0]);
		ret = true;
	}

	points[pCount][0] = x;
	points[pCount][1] = y;
	points[pCount][2] = z;
	pCount++;

	if (pCount > 1) {
		addLine(&points[0][0], &points[1][0]);
		ret = true;
	} 

	return ret;
}

void LineObject::nextLine() {
	// Select next Line
	if (iter != lines.end()) {
		iter->setColor(1.0, 1.0, 1.0);
		iter++;
		if (iter == lines.end()) iter--;
		iter->setColor(1.0, 0.0, 1.0);
	}
}

void LineObject::prevLine() {
	// Select previous Line
	if (iter != lines.end()) {
		iter->setColor(1.0, 1.0, 1.0);
		if (iter != lines.begin()) iter--;
		iter->setColor(1.0, 0.0, 1.0);
	}
}
