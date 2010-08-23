/*
 * debos.cpp
 *
 *  Created on: 22.08.2010
 *      Author: brainstar
 */

#include "debos.h"
#include "gl_drawer.h"
#include <stdio.h>

Debos::Debos() {
	this->resize(500,500);

	QMenuBar* menubar = new QMenuBar(this);
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

	this->setWindowTitle("debos");

	doc = NULL;
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
	if (doc) {
		QString filename = QFileDialog::getSaveFileName(this, "Save File", "~", "XML Files (*.xml)");
		if (!filename.isEmpty()) 
			doc->SaveFile(filename.toStdString().c_str());
	}
}

void Debos::closeFile() {
	if (doc)
		delete doc;
	doc = NULL;
}

void Debos::aboutDebos() {
	QMessageBox::information(this,"debos", "created by Christian Masser\n(c) 2010", "OK");
}
