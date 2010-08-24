/*
 * debos.cpp
 *
 *  Created on: 22.08.2010
 *      Author: brainstar
 */

#include "debos.h"
#include <stdio.h>
using namespace std;

Debos::Debos() {
	this->resize(500,500);

	QMenuBar* menubar = new QMenuBar();
	QAction* action;
	QMenu* menu;

	menu = menubar->addMenu("File");

	action = menu->addAction("New");
	connect(action, SIGNAL(triggered()), this, SLOT(newFile()));
	menu->addSeparator();

	action = menu->addAction("Save...");
	connect(action, SIGNAL(triggered()), this, SLOT(saveFile()));

	action = menu->addAction("Export...");
	connect(action, SIGNAL(triggered()), this, SLOT(exportFile()));

	action = menu->addAction("Load...");
	connect(action, SIGNAL(triggered()), this, SLOT(loadFile()));
	menu->addSeparator();

	action = menu->addAction("Close");
	connect(action, SIGNAL(triggered()), this, SLOT(closeFile()));
	menu->addSeparator();

	action = menu->addAction("Quit");
	connect(action, SIGNAL(triggered()), qApp, SLOT(quit()));

	menu = menubar->addMenu("?");

	action = menu->addAction("About debos...");
	connect(action, SIGNAL(triggered()), this, SLOT(aboutDebos()));

	gl = new GLDrawer();

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(menubar);
	layout->addWidget(gl, 1);
	layout->setSpacing(0);
	layout->setMargin(0);

	this->setLayout(layout);

	this->setWindowTitle("debos");

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
		float b[3] = { 5.0, 12.0, 0.0 };
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
}

void Debos::newFile() {
}

void Debos::loadFile() {
	QString filename = QFileDialog::getOpenFileName(this, "Open File", "test.xml", "XML Files (*.xml)");
	Document *doc = new Document;
	if (!filename.isEmpty()) {
		if (doc->load(filename.toStdString())) {
			delete data;
			gl->data = data = doc;
		}
		else
			delete doc;
	}
}

void Debos::exportFile() {
	QString filename = QFileDialog::getSaveFileName(this, "Export File", "~", "png (*.png)");
	if (!filename.isEmpty())
		gl->getScreen().save( filename, "png" );
}

void Debos::saveFile() {
	QString filename = QFileDialog::getSaveFileName(this, "Save File", "test.xml", "XML Files (*.xml)");
	if (!filename.isEmpty()) 
		data->save(filename.toStdString());
}

void Debos::closeFile() {
	delete data;
	gl->data = data = 0;
	gl->hide();
}

void Debos::aboutDebos() {
	QMessageBox::information(this,"debos", "created by Christian Masser\n(c) 2010", "OK");
}

void Debos::draw() {
	if (data) data->draw();
}
