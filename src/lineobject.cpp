#include "lineobject.h"
#include <stdio.h>
using namespace std;

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
}

void LineObject::deleteLine() {
	if (iter != lines.end()) {
		if ((iter = lines.erase(iter)) == lines.end())
			if (iter != lines.begin()) {
				iter--;
				iter->setColor(1.0, 0.0, 1.0);
			}
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
