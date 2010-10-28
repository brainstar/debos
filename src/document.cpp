#include "document.h"
#include <Qt/qwidget.h>
#include "tinyxml/tinyxml.h"
#define VERSION "0.2pre1"
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
	// Create XML tree and save file
	TiXmlDocument doc;
	{
		TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
		doc.LinkEndChild(decl);
	}
	TiXmlElement *root = new TiXmlElement("debos");
	doc.LinkEndChild(root);
	TiXmlElement *object;
	TiXmlElement *item;
	
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
		item->LinkEndChild(new TiXmlText("0.2pre1"));
		object->LinkEndChild(item);

		root->LinkEndChild(object);
	}

	object = new TiXmlElement("objects");
	
	// Bezier-Points
	{
		TiXmlElement* element;
		int i;
		for (list<Object*>::iterator iter = objects.begin(); iter != objects.end(); iter++) {
			i = 1;
			if ((*iter)->type == SPLINE) {
				element = new TiXmlElement("spline");
				SplineObject *so = (SplineObject*) (*iter);
				element->SetAttribute("closed", so->bClosed);
				list<BezierPoint>::iterator it;
				for (it = so->beziers.begin(); it != so->beziers.end(); it++) {
					float *p;
					item = new TiXmlElement("BezierPoint");
					item->SetAttribute("id", i);
					
					p = it->getpl();
					item->SetDoubleAttribute("l_x", *p);
					item->SetDoubleAttribute("l_y", *(p+1));
					
					p = it->getp();
					item->SetDoubleAttribute("p_x", *p);
					item->SetDoubleAttribute("p_y", *(p+1));
					
					p = it->getpr();
					item->SetDoubleAttribute("r_x", *p);
					item->SetDoubleAttribute("r_y", *(p+1));
					
					element->LinkEndChild(item);
					
					i++;
				}
				object->LinkEndChild(element);
			}
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
	return false;
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

		if (item = object->FirstChild("version")->ToElement())
			version = item->GetText();
		if (item = object->FirstChild("author")->ToElement())
			author = item->GetText();
		if (item = object->FirstChild("description")->ToElement())
			description = item->GetText();
	}

	if (!(object = hRoot.FirstChild("objects").ToElement())) return false;
	hObject = TiXmlHandle(object);
	
	if (version != VERSION)
		return false;

	// Splines
	{
		for (int i = 0; item = hObject.Child(i).ToElement(); i++) {
			if (item->ValueStr() == "spline") {
				addSplineObject();
				TiXmlHandle hItem(item);
				TiXmlElement *bezier;
				for (int j = 0; bezier = hItem.Child("BezierPoint", j).ToElement(); j++) {
					float p[3], l[3], r[3];
					p[2] = l[2] = r[2] = 0.;
					
					bezier->QueryFloatAttribute("p_x", &p[0]);
					bezier->QueryFloatAttribute("p_y", &p[1]);
					bezier->QueryFloatAttribute("l_x", &l[0]);
					bezier->QueryFloatAttribute("l_y", &l[1]);
					bezier->QueryFloatAttribute("r_x", &r[0]);
					bezier->QueryFloatAttribute("r_y", &r[1]);
					
					((SplineObject*)getObject())->addBezierPoint(p, l, r);
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
