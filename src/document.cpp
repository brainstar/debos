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
	so_iter->iterToEnd();
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
	so_iter->iterToEnd();
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


bool Document::save(string filename) {
	TiXmlDocument doc;
	{
		TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
		doc.LinkEndChild(decl);
	}
	TiXmlElement* root = new TiXmlElement("debos");
	doc.LinkEndChild(root);
	TiXmlElement* object;
	TiXmlElement* item;

	// Info
	{
		object = new TiXmlElement("info");

		item = new TiXmlElement("author");
		item->LinkEndChild(new TiXmlText(author.c_str()));
		object->LinkEndChild(item);

		item = new TiXmlElement("description");
		item->LinkEndChild(new TiXmlText(description.c_str()));
		object->LinkEndChild(item);

		item = new TiXmlElement("version");
		item->LinkEndChild(new TiXmlText("0.1pre4"));
		object->LinkEndChild(item);

		root->LinkEndChild(object);
	}

	object = new TiXmlElement("objects");

	// Splines
	{
		TiXmlElement* curve;
		int i;
		for (list<SplineObject>::iterator it = spline_objects.begin(); it != spline_objects.end(); it++) {
			curve = new TiXmlElement("curve");
			i = 1;
			list<Spline>::iterator ite;
			for (ite = it->splines.begin(); ite != it->splines.end(); ite++) {
				float *p;
				item = new TiXmlElement("pointpair");
				item->SetAttribute("id", i);

				p = ite->geta();
				item->SetDoubleAttribute("x1", *p);
				item->SetDoubleAttribute("y1", *(p + 1));
				p = ite->getk1();
				item->SetDoubleAttribute("x2", *p);
				item->SetDoubleAttribute("y2", *(p + 1));

				curve->LinkEndChild(item);
				i++;
			}
			if (ite != it->splines.begin()) {
				ite--;
				float *p1;
				float *p2;
				item = new TiXmlElement("pointpair");
				item->SetAttribute("id", i);

				p1 = ite->getk2();
				p2 = ite->getb();
				item->SetDoubleAttribute("x1", *p2);
				item->SetDoubleAttribute("y1", *(p2 + 1));
				item->SetDoubleAttribute("x2", *p2 * 2 - *p1);
				item->SetDoubleAttribute("y2", *(p2 + 1) * 2 - *(p1 + 1));

				curve->LinkEndChild(item);
			}
			object->LinkEndChild(curve);
		}		
	}
	// Lines
	{
		TiXmlElement *line;
		int i;
		for (list<LineObject>::iterator it = line_objects.begin(); it != line_objects.end(); it++) {
			line = new TiXmlElement("line");
			i = 1;
			list<Line>::iterator ite;
			for (ite = it->lines.begin(); ite != it->lines.end(); ite++) {
				float *p;
				item = new TiXmlElement("point");
				item->SetAttribute("id", i);

				p = ite->geta();
				item->SetDoubleAttribute("x", *p);
				item->SetDoubleAttribute("y", *(p + 1));

				line->LinkEndChild(item);
				i++;
			}
			if (ite != it->lines.begin()) {
				ite--;
				float *p;
				item = new TiXmlElement("point");
				item->SetAttribute("id", i);

				p = ite->getb();
				item->SetDoubleAttribute("x", *p);
				item->SetDoubleAttribute("y", *(p + 1));

				line->LinkEndChild(item);
			}
			object->LinkEndChild(line);
		}
	}

	root->LinkEndChild(object);

	// Grid
	{
		object = new TiXmlElement("grid");

		item = new TiXmlElement("x-axis");
		item->SetDoubleAttribute("lower", bounds[0]);
		item->SetDoubleAttribute("upper", bounds[1]);
		object->LinkEndChild(item);
		item = new TiXmlElement("y-axis");
		item->SetDoubleAttribute("lower", bounds[2]);
		item->SetDoubleAttribute("upper", bounds[3]);
		object->LinkEndChild(item);

		root->LinkEndChild(object);
	}

	// Now write the whole thing to disk.
	return doc.SaveFile(filename.c_str());
}

int Document::load(string filename) {
	return 1;
}


void Document::setGrid(float* bound) {
	bounds[0] = *bound;
	bounds[1] = *(bound + 1);
	bounds[2] = *(bound + 2);
	bounds[3] = *(bound + 3);
}

void Document::setAuthor(string aut) {
	author = aut;
}

void Document::setDescription(string des) {
	description = des;
}
