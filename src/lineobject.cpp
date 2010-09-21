#include "lineobject.h"
#include <stdio.h>
using namespace std;

LineObject::LineObject() {
	// Standard constructor
	for (int i = 0; i < 3; i++) {
		points[0][i] = points[1][i] = 0.0;
	}
	pCount = 0;
	activeLine = lines.end();
	type = LINE;
}

void LineObject::draw(bool edit) {
	// Draw the LineObject
	for (list<Line>::iterator it = lines.begin(); it != lines.end(); it++)
		it->draw(edit);
}

void LineObject::addInstance(float *a, float *k1, float *k2, float *b) { }

void LineObject::addInstance(float *a, float *b) {
	// Add a Line
	Line l(a, b);
	l.setColor(1.0, 0.0, 1.0);
	for(list<Line>::iterator it = lines.begin(); it != lines.end(); it++)
		it->setColor(1.0, 1.0, 1.0);
	lines.push_back(l);
	activeLine = lines.end();
	activeLine--;

	for (int i = 0; i < 3; i++) {
		points[0][i] = *(b + i);
	}
	pCount = 1;
}

void LineObject::deleteInstance() {
	// Delete selected Line
	list<Line>::iterator it = lines.end();
	if (it == lines.begin()) return;
	it--;
	if (activeLine == it || activeLine == lines.begin()) {
		activeLine = lines.erase(activeLine);
		if (activeLine == lines.end() && activeLine != lines.begin()) {
			activeLine--;
			activeLine->setColor(1.0, 0.0, 1.0);
		}
	}
}

bool LineObject::addPoint(float x, float y, float z) {
	// Append point to the LineObject
	bool ret = false;

	if(pCount > 1) {
		addInstance(&points[0][0], &points[1][0]);
		ret = true;
	}

	points[pCount][0] = x;
	points[pCount][1] = y;
	points[pCount][2] = z;
	pCount++;

	if (pCount > 1) {
		addInstance(&points[0][0], &points[1][0]);
		ret = true;
	} 

	return ret;
}

void LineObject::nextInstance() {
	// Select next Line
	if (activeLine != lines.end()) {
		activeLine->setColor(1.0, 1.0, 1.0);
		activeLine++;
		if (activeLine == lines.end()) activeLine--;
		activeLine->setColor(1.0, 0.0, 1.0);
	}
}

void LineObject::prevInstance() {
	// Select previous Line
	if (activeLine != lines.end()) {
		activeLine->setColor(1.0, 1.0, 1.0);
		if (activeLine != lines.begin()) activeLine--;
		activeLine->setColor(1.0, 0.0, 1.0);
	}
}

void LineObject::iterToEnd() {
	activeLine = lines.end();
	activeLine--;
}
