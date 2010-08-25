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
	layout->addStretch(0.5);
	layout->addWidget(gl, 1);
	layout->setSpacing(0);
	layout->setMargin(0);

	this->setLayout(layout);

	this->setWindowTitle("debos");

	data = 0;

	if (!loadFile("test.xml")) {
		qDebug("Going to create a new file");
		newFile();
	}

	gl->show();	
}

Debos::~Debos() {
}

void Debos::newFile() {
	gl->hide();
	qDebug("Widget hidden");
	if (data) delete data;
	qDebug("Data deletet");
	gl->data = data = new Document;
	qDebug("Data pointer set");
	gl->show();
	qDebug("Widget not hidden");
	mode = VIEW;
	qDebug("Creating new file");
}

bool Debos::loadFile() {
	QString filename = QFileDialog::getOpenFileName(this, "Open File", "test.xml", "XML Files (*.xml)");
	qDebug("Filename traced");
	return loadFile(filename);
}

bool Debos::loadFile(QString filename) {
	Document *doc = new Document;
	if (!filename.isEmpty()) {
		qDebug("Filename not empty");
		if (doc->load(filename.toStdString())) {
			qDebug("Loading successful");
			if (data) delete data;
			qDebug("Data deletet");
			gl->data = data = doc;
			qDebug("Data pointer set");
			gl->show();
			qDebug("gl->show();");
			mode = VIEW;
			qDebug("Mode set");
			return true;
		}
		else {
			delete doc;
			qDebug("Loading not successful");
		}
	}
	return false;
}

void Debos::exportFile() {
	QString filename = QFileDialog::getSaveFileName(this, "Export File", "~", "png (*.png)");
	if (!filename.isEmpty()) {
		gl->getScreen().save( filename, "png" );
		qDebug("Exporting file to %s", filename.toStdString().c_str());
	}
}

void Debos::saveFile() {
	QString filename = QFileDialog::getSaveFileName(this, "Save File", "test.xml", "XML Files (*.xml)");
	if (!filename.isEmpty())
		data->save(filename.toStdString());
}

void Debos::closeFile() {
	gl->hide();
	if (data) {
		delete data;
		qDebug("Closing file");
	}
	gl->data = data = 0;
}

void Debos::aboutDebos() {
	QMessageBox::information(this,"debos", "created by Christian Masser\n(c) 2010", "OK");
}

void Debos::draw() {
	if (data) data->draw();
}

void Debos::mouseClick(float x, float y) {
	if (mode == VIEW)
		mouseClickView(x, y);
	else if (mode == SPLINE)
		mouseClickSpline(x, y);
	else if (mode == LINE)
		mouseClickLine(x, y);
}

void Debos::keyPressEvent(QKeyEvent *event) {
	if (!data) {
		if (event->key() == Qt::Key_N)
			newFile();
	}
	else {
		if( event->key() == Qt::Key_V )
			activateViewMode();
		else if( event->key() == Qt::Key_S )
			activateSplineMode();
		else if( event->key() == Qt::Key_L )
			activateLineMode();

		else if (event->key() == Qt::Key_N) {
			if (mode == SPLINE) {
				data->addSplineObject();
				qDebug("adding SplineObject");
			}
			else if (mode == LINE) {
				data->addLineObject();
				qDebug("adding LineObject");
			}
		}
		else if (event->key() == Qt::Key_Delete) {
			if (mode == SPLINE && data->getSplineObject()) {
					data->getSplineObject()->deleteSpline();
				qDebug("deleting Spline");
			}
			else if (mode == LINE && data->getLineObject()) {
				data->getLineObject()->deleteLine();
				qDebug("deleting Line");
			}
		}
	}

	QWidget::keyPressEvent( event ); // important, default key handling
	
}

void Debos::activateViewMode() {
	if(mode != VIEW) {
		mode = VIEW;
		qDebug("activating view mode");
	}
}

void Debos::activateSplineMode() {
	if(mode != SPLINE) {
		mode = SPLINE;
		qDebug("activating spline mode");
	}
}

void Debos::activateLineMode() {
	if(mode != LINE) {
		mode = LINE;
		qDebug("activating line mode");
	}
}

void Debos::mouseClickView(float x, float y) {
}

void Debos::mouseClickSpline(float x, float y) {
	SplineObject *so;
	if (so = data->getSplineObject()) {
		so->addPoint(x, y, 0.0);	
	}
}

void Debos::mouseClickLine(float x, float y) {
}
