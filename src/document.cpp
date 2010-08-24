#include "document.h"
using namespace std;

void Document::draw() {
	for (list<SplineObject>::iterator it = spline_objects.begin(); it != spline_objects.end(); it++)
		it->draw();
}

void Document::addSplineObject() {
	SplineObject so;
	spline_objects.push_back(so);
	so_iter = spline_objects.end();
	so_iter--;
}

void Document::deleteSplineObject() {
	if (so_iter != spline_objects.end()) {
		spline_objects.erase(so_iter);
		if (so_iter == spline_objects.end())
			so_iter--;
	}
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
