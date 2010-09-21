#include "document.h"
#include <Qt/qwidget.h>
#include "tinyxml/tinyxml.h"
using namespace std;

Document::Document() {
	// Standard constructor to fill the file with some default data
	activeObject = objects.end();
	author = "author";
	description = "description";
	bounds[0] = bounds[2] = -10.0;
	bounds[1] = bounds[3] = 10.0;
}

void Document::draw(bool edit) {
	// Draw everything
	for (list<Object*>::iterator it = objects.begin(); it != objects.end(); it++)
		(*it)->draw(edit);
}

void Document::addSplineObject() {
	// Add a new SplineObject and select it
	Object *object = new SplineObject();
	objects.push_back(object);
	activeObject = objects.end();
	activeObject--;
}

void Document::deleteObject() {
	// Delete selected Object
	if (activeObject != objects.end()) {
		activeObject = objects.erase(activeObject);
		if (activeObject == objects.end())
			activeObject--;
		qDebug("removed Object");
	}
}

void Document::addLineObject() {
	// Add a new LineObject and select it
	Object* object = new LineObject;
	objects.push_back(object);
	activeObject = objects.end();
	activeObject--;
}

void Document::nextObject() {
	// Select next Object
	if (++activeObject == objects.end()) activeObject--;
	else qDebug("next Object");
}

void Document::prevObject() {
	// Select previous Object
	if (activeObject != objects.begin()) {
		activeObject--;
		qDebug("previous Object");
	}
}

Object* Document::getObject() {
	// Return pointer to the selected SplineObject
	if (activeObject == objects.end())
		return 0;
	return *activeObject;
}

bool Document::save(string filename) {
	// Create XML tree and save the file
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
		item->LinkEndChild(new TiXmlText("0.1"));
		object->LinkEndChild(item);

		root->LinkEndChild(object);
	}

	object = new TiXmlElement("objects");

	// Splines and Lines
	{
		TiXmlElement* element;
		int i;
		for (list<Object*>::iterator iter = objects.begin(); iter != objects.end(); iter++) {
			i = 1;
			if ((*iter)->type == SPLINE) {
				element = new TiXmlElement("curve");
				SplineObject* so = (SplineObject*) (*iter);
				list<Spline>::iterator it;
				for (it = so->splines.begin(); it != so->splines.end(); it++) {
					float *p;
					item = new TiXmlElement("pointpair");
					item->SetAttribute("id", i);

					p = it->geta();
					item->SetDoubleAttribute("x1", *p);
					item->SetDoubleAttribute("y1", *(p+1));
					p = it->getk1();
					item->SetDoubleAttribute("x2", *p);
					item->SetDoubleAttribute("y2", *(p+1));

					element->LinkEndChild(item);
					i++;
				}
				if (it != so->splines.begin()) {
					it--;
					float *p1;
					float *p2;
					item = new TiXmlElement("pointpair");
					item->SetAttribute("id", i);

					p1 = it->getk2();
					p2 = it->getb();
					item->SetDoubleAttribute("x1", *p2);
					item->SetDoubleAttribute("y1", *(p2 + 1));
					item->SetDoubleAttribute("x2", *p2 * 2 - *p1);
					item->SetDoubleAttribute("y2", *(p2 + 1) * 2 - *(p1 + 1));
				}
			}
			else if ((*iter)->type == LINE) {
				element = new TiXmlElement("line");
				LineObject* lo = (LineObject*) (*iter);
				list<Line>::iterator it;
				for (it = lo->lines.begin(); it != lo->lines.end(); it++) {
					float *p;
					item = new TiXmlElement("point");
					item->SetAttribute("id", i);

					p = it->geta();
					item->SetDoubleAttribute("x", *p);
					item->SetDoubleAttribute("y", *(p + 1));

					element->LinkEndChild(item);
					i++;
				}
				if (it != lo->lines.begin()) {
					it--;
					float *p;
					item = new TiXmlElement("point");
					item->SetAttribute("id", i);

					p = it->getb();
					item->SetDoubleAttribute("x", *p);
					item->SetDoubleAttribute("y", *(p + 1));

					element->LinkEndChild(item);
				}
			}
			object->LinkEndChild(element);
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

	// Write the whole thing to disk
	return doc.SaveFile(filename.c_str());
}

bool Document::load(string filename) {
	// Load XML-File and save it into a Document instance
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

	// Splines and Lines
	{
		for (int i = 0; item = hObject.Child(i).ToElement(); i++) {
			if (item->ValueStr() == "curve") {
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

				getObject()->addInstance(a, b, c, d);

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

					getObject()->addInstance(a, b, c, d);
				}
			}
			else if (item->ValueStr() == "line") {
				addLineObject();
				TiXmlHandle hItem(item);
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
					getObject()->addInstance(a, b);
				}
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

	return true;
}


void Document::setGrid(float* bound) {
	// Set the grid
	for (int i = 0; i < 4; i++)
		bounds[i] = bound[i];
}

void Document::setAuthor(string aut) {
	// Set the author
	author = aut;
}

void Document::setDescription(string des) {
	// Set the description
	description = des;
}
