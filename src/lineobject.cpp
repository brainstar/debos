#include "lineobject.h"
#include <stdio.h>
using namespace std;

LineObject::LineObject() {
	for (int i = 0; i < 3; i++) {
		points[0][i] = points[1][i] = 0.0;
	}
	pCount = 0;
	iter = lines.end();
}

void LineObject::draw() {
	for (list<Line>::iterator it = lines.begin(); it != lines.end(); it++)
		it->draw();
}

void LineObject::addLine(float *a, float *b) {
	Line l(a, b);
	l.setColor(1.0, 0.0, 1.0);
	for(list<Line>::iterator it = lines.begin(); it != lines.end(); it++)
		it->setColor(1.0, 1.0, 1.0);
	lines.push_back(l);
	iter = lines.end();
	iter--;

	for (int i = 0; i < 3; i++) {
		points[0][i] = points[1][i];
	}
	pCount = 1;
}

void LineObject::deleteLine() {
	list<Line>::iterator it = lines.end();
	it--;
	if (iter == it || iter == lines.begin()) {
		iter = lines.erase(iter);
		if (iter == lines.end() && iter != lines.begin()) {
			iter--;
			iter->setColor(1.0, 0.0, 1.0);
		}
	}
}

void LineObject::addPoint(float x, float y, float z) {
	if(pCount > 1) {
		addLine(&points[0][0], &points[1][0]);
	}

	points[pCount][0] = x;
	points[pCount][1] = y;
	points[pCount][2] = z;
	pCount++;

	if (pCount > 3) {
		addLine(&points[0][0], &points[1][0]);
	} 
}

void LineObject::nextLine() {
	if (iter != lines.end()) {
		iter->setColor(1.0, 1.0, 1.0);
		if (iter != lines.begin()) iter++;
		iter->setColor(1.0, 0.0, 1.0);
	}
}

void LineObject::prevLine() {
	if (iter != lines.end()) {
		iter->setColor(1.0, 1.0, 1.0);
		if (iter != lines.begin()) iter--;
		iter->setColor(1.0, 0.0, 1.0);
	}
}
