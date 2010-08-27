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

	connect( gl, SIGNAL(mouseClicked(float, float)), this, SLOT(mouseClick(float, float)) );

	gl->show();	
}

Debos::~Debos() {
}

void Debos::newFile() {
	gl->hide();
	if (data) delete data;
	gl->data = data = new Document;
	gl->show();
	mode = VIEW;
	qDebug("Creating new file");
}

bool Debos::loadFile() {
	QString filename = QFileDialog::getOpenFileName(this, "Open File", "test.xml", "XML Files (*.xml)");
	return loadFile(filename);
}

bool Debos::loadFile(QString filename) {
	Document *doc = new Document;
	if (!filename.isEmpty()) {
		if (doc->load(filename.toStdString())) {
			if (data) delete data;
			gl->data = data = doc;
			gl->simResize();
			gl->show();
			mode = VIEW;
			qDebug("Loading successful");
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
	QMessageBox::information(this,"debos", "created by:\nMike Duenbostell,\nChristian Masser\n(c) 2010", "OK");
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
		if(event->key() == Qt::Key_V)
			activateMode(VIEW);
		else if(event->key() == Qt::Key_S)
			activateMode(SPLINE);
		else if(event->key() == Qt::Key_L)
			activateMode(LINE);

		else if (mode == VIEW) {
			if (event->key() == Qt::Key_Plus) {
				float *g = data->getGrid();
				float w = (*(g+1)) - (*g);
				float h = (*(g+3)) - (*(g+2));
				*g += w / 40.0;
				*(g+1) -= w / 40.0;
				*(g+2) += h / 40.0;
				*(g+3) -= h / 40.0;
				gl->simResize();
			}
			if (event->key() == Qt::Key_Minus) {
				float *g = data->getGrid();
				float w = (*(g+1)) - (*g);	
				float h = (*(g+3)) - (*(g+2));
				*g -= w / 40.0;
				*(g+1) += w / 40.0;
				*(g+2) -= h / 40.0;
				*(g+3) += h / 40.0;
				gl->simResize();
			}
			else if (event->key() == Qt::Key_Left) {
				float *g = data->getGrid();
				float w = (*(g+1)) - (*g);
				*g -= w / 20.0;
				*(g + 1) -= w / 20.0;
				gl->simResize();
			}
			else if (event->key() == Qt::Key_Right) {
				float *g = data->getGrid();
				float w = (*(g+1)) - (*g);
				*g += w / 20.0;
				*(g + 1) += w / 20.0;
				gl->simResize();
			}
			else if (event->key() == Qt::Key_Down) {
				float *g = data->getGrid();
				float h = (*(g+3)) - (*(g+2));
				*(g + 2) -= h / 20.0;
				*(g + 3) -= h / 20.0;
				gl->simResize();
			}
			else if (event->key() == Qt::Key_Up) {
				float *g = data->getGrid();
				float h = (*(g+3)) - (*(g+2));
				*(g + 2) += h / 20.0;
				*(g + 3) += h / 20.0;
				gl->simResize();
			}
		}
		else if (mode == SPLINE) {
			if (event->key() == Qt::Key_N) {
				data->addSplineObject();
				qDebug("adding SplineObject");
			}
			else if (event->key() == Qt::Key_X) {
				data->deleteSplineObject();
				qDebug("removed SplineObject");
			}
			else if (event->key() == Qt::Key_Left) {
				if (data->getSplineObject()) {
					data->getSplineObject()->prevSpline();
				}
			}
			else if (event->key() == Qt::Key_Right) {
				if (data->getSplineObject()) {
					data->getSplineObject()->nextSpline();
				}
			}
			else if (event->key() == Qt::Key_Down) {
				data->prevSplineObject();
			}
			else if (event->key() == Qt::Key_Up) {
				data->nextSplineObject();
			}
			else if (event->key() == Qt::Key_Delete && data->getSplineObject()) {
				data->getSplineObject()->deleteSpline();
			}
		}
		else if (mode == LINE) {
			if (event->key() == Qt::Key_N) {
				data->addLineObject();
				qDebug("adding LineObject");
			}
			else if (event->key() == Qt::Key_X) {
				data->deleteLineObject();
				qDebug("removed LineObject");
			}
			else if (event->key() == Qt::Key_Left) {
				if (data->getLineObject())
					data->getLineObject()->prevLine();
			}
			else if (event->key() == Qt::Key_Right) {
				if (data->getLineObject())
					data->getLineObject()->nextLine();
			}
			else if (event->key() == Qt::Key_Down) {
				data->prevLineObject();
			}
			else if (event->key() == Qt::Key_Up) {
				data->nextLineObject();
			}
			else if (event->key() == Qt::Key_Delete && data->getLineObject()) {
				data->getLineObject()->deleteLine();
			}
		}
	}

	QWidget::keyPressEvent( event ); // important, default key handling
	gl->updateGL();
}

void Debos::activateMode(Mode pMode) {
	if(mode != pMode) {
		mode = pMode;
		qDebug("activating mode %d", mode);
	}
}

void Debos::mouseClickView(float x, float y) {
}

void Debos::mouseClickSpline(float x, float y) {
	SplineObject *so;
	if (so = data->getSplineObject()) {
		so->addPoint(x, y, 0.0);
		gl->updateGL();
	}
}

void Debos::mouseClickLine(float x, float y) {
	LineObject *lo;
	if (lo = data->getLineObject()) {
		qDebug("lo: %i", lo);
		lo->addPoint(x, y, 0.0);
		gl->updateGL();
	}
}
