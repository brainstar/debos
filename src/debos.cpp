/*
 * debos.cpp
 *
 *  Created on: 22.08.2010
 *      Author: brainstar
 */

#include "debos.h"
#include "gl_drawer.h"
#include <stdio.h>
using namespace std;

Debos::Debos() {
	this->resize(500,500);

	QMenuBar* menubar = new QMenuBar();
	QAction* action;
	QMenu* menu;

	menu = menubar->addMenu("File");

	action = menu->addAction("New");
	QObject::connect(action, SIGNAL(triggered()), this, SLOT(newFile()));
	menu->addSeparator();

	action = menu->addAction("Save...");
	QObject::connect(action, SIGNAL(triggered()), this, SLOT(saveFile()));
	action = menu->addAction("Load...");
	QObject::connect(action, SIGNAL(triggered()), this, SLOT(loadFile()));
	menu->addSeparator();

	action = menu->addAction("Close");
	QObject::connect(action, SIGNAL(triggered()), this, SLOT(closeFile()));
	menu->addSeparator();

	action = menu->addAction("Quit");
	QObject::connect(action, SIGNAL(triggered()), qApp, SLOT(quit()));

	menu = menubar->addMenu("?");

	action = menu->addAction("About debos...");
	QObject::connect(action, SIGNAL(triggered()), this, SLOT(aboutDebos()));

	GLDrawer *gl = new GLDrawer();

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(menubar);
	layout->addWidget(gl, 1);
	layout->setSpacing(0);
	layout->setMargin(0);

	this->setLayout(layout);

	this->setWindowTitle("debos");

	doc = NULL;
	gl->data = data = new Document;

	data->addSplineObject();
	{
		float a[3] = { -4.0, -4.0, 0.0 };
		float b[3] = { -3.0, 4.0, 0.0 };
		float c[3] = { 3.0, -4.0, 0.0 };
		float d[3] = { 4.0, 4.0, 0.0 };
		data->getSplineObject()->addSpline(a, b, c, d);
	}
	{
		float a[3] = { 4.0, 4.0, 0.0 };
		float b[3] = { 5.0, 8.0, 0.0 };
		float c[3] = { -4.0, -4.0, 0.0 };
		float d[3] = { -4.0, 0.0, 0.0 };
		data->getSplineObject()->addSpline(a, b, c, d);
	}
	{
		float a[3] = { -4.0, 0.0, 0.0 };
		float b[3] = { -4.0, 4.0, 0.0 };
		float c[3] = { 4.0, 4.0, 0.0 };
		float d[3] = { 0.0, 8.0, 0.0 };
		data->getSplineObject()->addSpline(a, b, c, d);
	} 
	data->addLineObject();
	{
		float a[3] = { -6.0, 6.0, 0.0 };
		float b[3] = { 6.0, -6.0, 0.0 };
		float c[3] = { -6.0, -6.0, 0.0 };
		float d[3] = { -6.0, -5.0, 0.0 };
		data->getLineObject()->addLine(a, b);
		data->getLineObject()->addLine(b, c);
		data->getLineObject()->addLine(c, d);
	}
	
	{
		float bounds[4] = { -10.0, 10.0, -10.0, 10.0 };
		data->setGrid(bounds);
		gl->grid = data->getGrid();
		data->setAuthor("brainstar");
		data->setDescription("test document");
	}
	gl->show();
}

Debos::~Debos() {
	delete doc;
	doc = NULL;
}

void Debos::newFile() {
	if (doc)
		delete doc;
	doc = new TiXmlDocument;
}

void Debos::loadFile() {
	QString filename = QFileDialog::getOpenFileName(this, "Open File", "~", "XML Files (*.xml)");
	if (!filename.isEmpty()) {
		if (doc)
			delete doc;
		doc = new TiXmlDocument(filename.toStdString().c_str());
		if (doc->LoadFile()) {
			//TODO: Loading File-Code comes here
		}
		else {
			QMessageBox msgBox;
			msgBox.setText("Error parsing file!");
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.setIcon(QMessageBox::Warning);
			msgBox.exec();
			delete doc;
			doc = NULL;
		}
	}
}

void Debos::saveFile() {
	QString filename = QFileDialog::getSaveFileName(this, "Save File", "test.xml", "XML Files (*.xml)");
	if (!filename.isEmpty()) 
		data->save(filename.toStdString());
}

void Debos::closeFile() {
	if (doc)
		delete doc;
	doc = NULL;
}

void Debos::aboutDebos() {
	QMessageBox::information(this,"debos", "created by Christian Masser\n(c) 2010", "OK");
}

void Debos::draw() {
	if (data) data->draw();
}
