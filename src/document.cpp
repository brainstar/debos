#include "document.h"
using namespace std;

void Document::draw() {
	for (list<SplineObject>::iterator it = spline_objects.begin(); it != spline_objects.end(); it++)
		it->draw();
	for(list<LineObject>::iterator it = line_objects.begin(); it != line_objects.end(); it++)
		it->draw();
}

void Document::addSplineObject() {
	SplineObject so;
	spline_objects.push_back(so);
	so_iter = spline_objects.end();
	so_iter--;
}

void Document::deleteSplineObject() {
	if (so_iter != spline_objects.end())
		if ((so_iter = spline_objects.erase(so_iter)) == spline_objects.end() && so_iter != spline_objects.begin())
			so_iter--;
}

void Document::addLineObject() {
	LineObject lo;
	line_objects.push_back(lo);
	l_iter = line_objects.end();
	l_iter--;
}

void Document::deleteLineObject() {
	if (l_iter != line_objects.end())
		if ((l_iter = line_objects.erase(l_iter)) == line_objects.end() && l_iter != line_objects.begin())
			l_iter--;
}

void Document::nextSplineObject() {
	if (++so_iter == spline_objects.end()) so_iter--;
}

void Document::prevSplineObject() {
	if (so_iter != spline_objects.begin()) so_iter--;
}


SplineObject* Document::getSplineObject() {
	return &(*so_iter);
}

void Document::nextLineObject() {
	if(++l_iter == line_objects.end()) l_iter--;
}

void Document::prevLineObject() {
	if(l_iter != line_objects.begin()) l_iter--;
}

LineObject* Document::getLineObject() {
	return &(*l_iter);
}
