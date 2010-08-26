#include "document.h"
#include <Qt/qwidget.h>
using namespace std;

Document::Document() {
	so_iter = spline_objects.end();
	l_iter = line_objects.end();
	author = "author";
	description = "description";
	bounds[0] = bounds[2] = -10.0;
	bounds[1] = bounds[3] = 10.0;
}

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
	if (so_iter != spline_objects.end()) {
		so_iter = spline_objects.erase(so_iter);
		if (so_iter == spline_objects.end() && so_iter != spline_objects.begin())
			so_iter--;
	}
}

void Document::addLineObject() {
	LineObject lo;
	line_objects.push_back(lo);
	l_iter = line_objects.end();
	l_iter--;
	l_iter->iterToEnd();
}

void Document::deleteLineObject() {
	if (l_iter != line_objects.end()) {
		l_iter = line_objects.erase(l_iter);
		if (l_iter == line_objects.end() && l_iter != line_objects.begin())
			l_iter--;
	}
}

void Document::nextSplineObject() {
	if (++so_iter == spline_objects.end()) so_iter--;
	else qDebug("next SplineObject");
}

void Document::prevSplineObject() {
	if (so_iter != spline_objects.begin()) {
		so_iter--;
		qDebug("previous SplineObject");
	}
}


SplineObject* Document::getSplineObject() {
	if (so_iter == spline_objects.end())
		return 0;
	return &(*so_iter);
}

void Document::nextLineObject() {
	if(++l_iter == line_objects.end()) l_iter--;
	else qDebug("next LineObject");
}

void Document::prevLineObject() {
	if(l_iter != line_objects.begin()) l_iter--;
	else qDebug("previous LineObject");
}

LineObject* Document::getLineObject() {
	if (l_iter == line_objects.end())
		return 0;
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

bool Document::load(string filename) {
	TiXmlDocument doc(filename.c_str());
	if (!doc.LoadFile())
		return false;

	TiXmlHandle hDoc(&doc);
	TiXmlElement *object;
	TiXmlElement *item;
	TiXmlHandle hRoot(0);
	TiXmlHandle hObject(0);

	object = hDoc.FirstChildElement().Element();
	if (!object) return false;
	hRoot = TiXmlHandle(object);

	// Information
	{
		object = hRoot.FirstChild("info").ToElement();
		if (!object) return false;

		if (item = object->FirstChild("author")->ToElement())
			author = item->GetText();
		if (item = object->FirstChild("description")->ToElement())
			description = item->GetText();
	}

	if (!(object = hRoot.FirstChild("objects").ToElement())) return false;
	hObject = TiXmlHandle(object);

	// Splines
	{
		for (int i = 0; item = hObject.Child("curve" ,i).ToElement(); i++) {
			addSplineObject();
			TiXmlHandle hItem(item);
			TiXmlElement *spline = hItem.Child("pointpair", 0).ToElement();
			if (!spline) break;

			float a[3], b[3], c[3], d[3];
			for (int n = 0; n < 3; n++)
				a[n] = b[n] = c[n] = d[n] = 0.0;

			spline->QueryFloatAttribute("x1", a);
			spline->QueryFloatAttribute("y1", &a[1]);
			spline->QueryFloatAttribute("x2", b);
			spline->QueryFloatAttribute("y2", &b[1]);

			spline = hItem.Child("pointpair", 1).ToElement();
			if (!spline) break;

			spline->QueryFloatAttribute("x1", d);
			spline->QueryFloatAttribute("y1", &d[1]);
			spline->QueryFloatAttribute("x2", c);
			spline->QueryFloatAttribute("y2", &c[1]);
			c[0] = 2*d[0] - c[0];
			c[1] = 2*d[1] - c[1];

			getSplineObject()->addSpline(a, b, c, d);

			for (int j = 2; spline = hItem.Child("pointpair", j).ToElement(); j++) {
				for (int k = 0; k < 3; k++) {
					a[k] = d[k];
					b[k] = 2*d[k] - c[k];
				}

          	                spline->QueryFloatAttribute("x1", d);
                	        spline->QueryFloatAttribute("y1", &d[1]);
                        	spline->QueryFloatAttribute("x2", c);
                	        spline->QueryFloatAttribute("y2", &c[1]);
        	                c[0] = 2*d[0] - c[0];
	                        c[1] = 2*d[1] - c[1];

				getSplineObject()->addSpline(a, b, c, d);
			}
		}
	} 

	// Lines
	{
		for (int i = 0; item = hObject.Child("line", i).ToElement(); i++) {
			TiXmlHandle hItem(item);
			addLineObject();
			TiXmlElement *line = hItem.Child("point", 0).ToElement();
			if (!line) break;

			float a[3], b[3];
			for (int n = 0; n < 3; n++)
				a[n] = b[n] = 0;

			line->QueryFloatAttribute("x", b);
			line->QueryFloatAttribute("y", &b[1]);
			
			for (int j = 1; line = hItem.Child("point", j).ToElement(); j++) {
				for (int k = 0; k < 3; k++)
					a[k] = b[k];
				line->QueryFloatAttribute("x", b);
				line->QueryFloatAttribute("y", &b[1]);
				getLineObject()->addLine(a, b);
			}
		}
	}

	// Grid
	{
		if (object = hRoot.FirstChild("grid").ToElement()) {
			hObject = TiXmlHandle(object);
			if (item = hObject.FirstChild("x-axis").ToElement()) {
				item->QueryFloatAttribute("lower", bounds);
				item->QueryFloatAttribute("upper", &bounds[1]);
			}
			else {
				bounds[0] = -10.0;
				bounds[1] = 10.0;
			}
			if (item = hObject.FirstChild("y-axis").ToElement()) {
				item->QueryFloatAttribute("lower", &bounds[2]);
				item->QueryFloatAttribute("upper", &bounds[3]);
			}
			else {
				bounds[2] = -10.0;
				bounds[3] = 10.0;
			}
		}
		else {
			bounds[0] = bounds[2] = -10.0;
			bounds[1] = bounds[3] = 10.0;
		}
	}

	printf("\n%s\n%s\n%s\n", author.c_str(), description.c_str(), filename.c_str());
	printf("\nGrid:\n%f/%f .. %f %f\n", bounds[0], bounds[1], bounds[2], bounds[3]);
	printf("\nSplineObjects: \n");
	for (list<SplineObject>::iterator a = spline_objects.begin(); a != spline_objects.end(); a++) {
		printf("+ Splines: \n");
		for (list<Spline>::iterator b = a->splines.begin(); b != a->splines.end(); b++) {
			printf("  + Spline %f/%f .. %f/%f .. %f/%f .. %f/%f\n", *(b->geta()), *(b->geta()+1), *(b->getk1()), *(b->getk1()+1),
				*(b->getk2()), *(b->getk2()+1), *(b->getb()), *(b->getb()+1));
		}
	}
	printf("\nLineObjects: \n");
	for (list<LineObject>::iterator a = line_objects.begin(); a != line_objects.end(); a++) {
		printf("+ Lines: \n");
		for (list<Line>::iterator b = a->lines.begin(); b != a->lines.end(); b++) {
			printf("  + Line %f/%f .. %f/%f\n", *(b->geta()), *(b->geta()+1), *(b->getb()), *(b->getb()+1));
		}
	}

	return true;
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
