/*
 * debos.cpp
 *
 *  Created on: 22.08.2010
 *      Author: Christian M.
 */

#include "debos.h"
#include <stdio.h>
using namespace std;

Debos::Debos() {
	// Initializing interactive variables
	setMouseTracking(true);
	grabMouse();
	bView = false;
	bGrab = false;
	bRotate = false;
	bScale = false;

	// Creating GUI
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

	// Loading default file
	data = 0;

	if (!loadFile("test.xml")) {
		qDebug("Going to create a new file");
		newFile();
	}

	connect( gl, SIGNAL(mouseClicked(float, float)), this, SLOT(mouseClick(float, float)) );
	connect(gl, SIGNAL(mouseMoved(int, int)), this, SLOT(mouseMove(int, int)));
	connect(gl, SIGNAL(draw()), this, SLOT(draw()));

	gl->show();	
}

Debos::~Debos() {
}

void Debos::newFile() {
	// Creat empty file
	gl->hide();
	if (data) delete data;
	gl->data = data = new Document;
	gl->show();
	gl->simResize();
	mode = VIEW;
	qDebug("Creating new file");
}

bool Debos::loadFile() {
	// Load file with dialog
	QString filename = QFileDialog::getOpenFileName(this, "Open File", "test.xml", "XML Files (*.xml)");
	return loadFile(filename);
}

bool Debos::loadFile(QString filename) {
	// Load file
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
	// Export file to *.png image
	QString filename = QFileDialog::getSaveFileName(this, "Export File", "~", "png (*.png)");
	if (!filename.isEmpty()) {
		gl->getScreen().save( filename, "png" );
		qDebug("Exporting file to %s", filename.toStdString().c_str());
	}
}

void Debos::saveFile() {
	// Save file to *.xml file
	QString filename = QFileDialog::getSaveFileName(this, "Save File", "test.xml", "XML Files (*.xml)");
	if (!filename.isEmpty())
		data->save(filename.toStdString());
}

void Debos::closeFile() {
	// Close file without saving
	gl->hide();
	if (data) {
		delete data;
		qDebug("Closing file");
	}
	gl->data = data = 0;
}

void Debos::aboutDebos() {
	// Display "About"-window
	QMessageBox::information(this,"debos", "created by:\nMike Duenbostell,\nChristian Masser\n(c) 2010", "OK");
}

void Debos::draw() {
	// Draw all elements of the file
	if (!data) return;
	if (mode == VIEW)
		data->draw(false);
	else if (mode == EDIT)
		data->draw(true);
}

void Debos::mouseClick(float x, float y) {
	// Event handler for mouse click
	if (mode == VIEW)
		mouseClickView(x, y);
	else if (mode == EDIT)
		mouseClickEdit(x, y);
}

void Debos::mouseMove(int ix, int iy) {
	if (data) {
		if (bGrab) {
			float x, y;
			gl->ptc(ix, iy, &x, &y);
			grab(iMouse[0], iMouse[1], x, y);
		}
		else if (bRotate) {
			float x, y;
			gl->ptc(ix, iy, &x, &y);
			rotate(iMouse[0], iMouse[1], x, y);
		}
		else if (bScale) {
			float x, y;
			gl->ptc(ix, iy, &x, &y);
			scale(iMouse[0], iMouse[1], x, y);
		}
		gl->updateGL();
	}
	gl->ptc(ix, iy, &iMouse[0], &iMouse[1]);
}

void Debos::mouseMoveEvent(QMouseEvent *event) {
	mouseMove(event->globalX(), event->globalY());
	QWidget::mouseMoveEvent(event);
}

void Debos::keyPressEvent(QKeyEvent *event) {
	// Event handler for keyboard stroke
	if (!data) {
		if (event->key() == Qt::Key_N)
			newFile();
	}
	else {			
		// Actions not related to specific modes use the Shift modifier
		if (event->modifiers() & Qt::ControlModifier) {
			// Activate modes
			if(event->key() == Qt::Key_V)
				activateMode(VIEW);
			else if(event->key() == Qt::Key_E)
				activateMode(EDIT);

			if (bView) {
				// Move camera to the left
				if (event->key() == Qt::Key_Left) {
					float *g = data->getGrid();
					float w = (*(g+1)) - (*g);
					*g -= w / 20.0;
					*(g + 1) -= w / 20.0;
					gl->simResize();
				}

				// Move camera to the right
				else if (event->key() == Qt::Key_Right) {
					float *g = data->getGrid();
					float w = (*(g+1)) - (*g);
					*g += w / 20.0;
					*(g + 1) += w / 20.0;
					gl->simResize();
				}

				// Move camera down
				else if (event->key() == Qt::Key_Down) {
					float *g = data->getGrid();
					float h = (*(g+3)) - (*(g+2));
					*(g + 2) -= h / 20.0;
					*(g + 3) -= h / 20.0;
					gl->simResize();
				}

				// Move camera up
				else if (event->key() == Qt::Key_Up) {
					float *g = data->getGrid();
					float h = (*(g+3)) - (*(g+2));
					*(g + 2) += h / 20.0;
					*(g + 3) += h / 20.0;
					gl->simResize();
				}

				// Zoom in
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
			
				// Zoom out
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
			}
		}

		// View mode is active
		else if (mode == VIEW) {
			if (event->key() == Qt::Key_C) {
				closeFile();
			}
			// Select previous Object
			else if (event->key() == Qt::Key_Down) {
				data->prevObject();
			}
			// Select next Object
			else if (event->key() == Qt::Key_Up) {
				data->nextObject();
			}
			// Add a SplineObject
			else if (event->key() == Qt::Key_S) {
				data->addSplineObject();
				qDebug("added SplineObject");
			}
			// Delete an Object
			else if (event->key() == Qt::Key_X) {
				data->deleteObject();
			}
		}
		
		// Edit mode is active
		else if (mode == EDIT) {
			if (event->key() == Qt::Key_Escape) {
				if (bGrab) {
					grab(iMouse[0], iMouse[1], iPos[0], iPos[1]);
					bGrab = false;
				}
				else if (bRotate) {
					rotate(iMouse[0], iMouse[1], iPos[0], iPos[1]);
					bRotate = false;
					bView = false;
				}
				else if (bScale) {
					scale(iMouse[0], iMouse[1], iPos[0], iPos[1]);
					bScale = false;
					bView = false;
				}
			}
			// Alt Modifier is used for moving operations
			else if (event->modifiers() & Qt::AltModifier) {
				// Move BezierPoint
				if (event->key() == Qt::Key_Down) {
					Object *obj = data->getObject();
					if (obj) {
						if (obj->type == SPLINE)
							((SplineObject*)obj)->moveBezierPoint(0., -1.);
					}
				}
				
				if (event->key() == Qt::Key_Up) {
					Object *obj = data->getObject();
					if (obj) {
						if (obj->type == SPLINE)
							((SplineObject*)obj)->moveBezierPoint(0., 1.);
					}
				}
				
				if (event->key() == Qt::Key_Left) {
					Object *obj = data->getObject();
					if (obj) {
						if (obj->type == SPLINE)
							((SplineObject*)obj)->moveBezierPoint(-1., 0.);
					}
				}
				
				if (event->key() == Qt::Key_Right) {
					Object *obj = data->getObject();
					if (obj) {
						if (obj->type == SPLINE)
							((SplineObject*)obj)->moveBezierPoint(1., 0.);
					}
				}
			}
			
			else {
				// Move BezierPoint
				if (event->key() == Qt::Key_G) {
					Object *obj = data->getObject();
					if (obj) {
						if (obj->type == SPLINE) {
							SplineObject *so = (SplineObject*) obj;
							if (so->beziers.begin() != so->beziers.end()) {
								bGrab = true;
								iPos[0] = iMouse[0];
								iPos[1] = iMouse[1];
							}
						}
					}
				}
				
				// Rotate BezierPoint
				else if (event->key() == Qt::Key_R) {
					Object *obj = data->getObject();
					if (obj) {
						if (obj->type == SPLINE) {
							SplineObject *so = (SplineObject*) obj;
							if (so->beziers.begin() != so->beziers.end()) {
								bRotate = true;
								bView = false;
								iPos[0] = iMouse[0];
								iPos[1] = iMouse[1];
							}
						}
					}
				}
				
				// Scale BezierPoint
				else if (event->key() == Qt::Key_S) {
					Object *obj = data->getObject();
					if (obj) {
						if (obj->type == SPLINE) {
							SplineObject *so = (SplineObject*) obj;
							if (so->beziers.begin() != so->beziers.end()) {
								bScale = true;
								bView = false;
							}
						}
					}
				}
				// Close Bezier-Curve
				else if (event->key() == Qt::Key_C) {
					Object *obj = data->getObject();
					if (obj) {
						if (obj->type == SPLINE) {
							SplineObject *so = (SplineObject*) obj;
							so->bClosed = !(so->bClosed);
							so->computeSplines();
						}
					}
				}
				
				// Select previous BezierPoint
				else if (event->key() == Qt::Key_Down) {
					Object *obj = data->getObject();
					if (obj) {
						obj->prevInstance();
					}
				}
			
				// Select next BezierPoint
				else if (event->key() == Qt::Key_Up) {
					Object *obj = data->getObject();
					if (obj) {
						obj->nextInstance();
					}
				}
			
				// Turn BezierPoint
				else if (event->key() == Qt::Key_Left) {
					Object *obj = data->getObject();
					if (obj) {
						if (obj->type == SPLINE)
							((SplineObject*)obj)->turnBezierPoint(10.);
					}
				}
				else if (event->key() == Qt::Key_Right) {
					Object *obj = data->getObject();
					if (obj) {
						if (obj->type == SPLINE)
							((SplineObject*)obj)->turnBezierPoint(-10.);
					}
				}
			
				// Delete BezierPoint
				else if (event->key() == Qt::Key_Delete) {
					Object *obj = data->getObject();
					if (obj) {
						obj->deleteInstance();
						gl->updateGL();
					}
				}
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

void Debos::mouseClickEdit(float x, float y) {
	if (bGrab) {
		bGrab = false;
	}
	else if (bRotate) {
		bRotate = false;
		bView = false;
	}
	else if (bScale) {
		bScale = false;
		bView = false;
	}
	else {
		Object *obj;
		if (obj = data->getObject()) {
			obj->addPoint(x, y, 0.0);
			gl->updateGL();
		}
	}
}

void Debos::grab(float FromX, float FromY, float ToX, float ToY) {
	float diff[2];
	diff[0] = ToX - FromX;
	diff[1] = ToY - FromY;
	Object *obj = data->getObject();
	if (obj) {
		if (obj->type == SPLINE) {
			SplineObject *so = (SplineObject*) obj;
			so->moveBezierPoint(diff[0], diff[1]);
		}
	}
}

void Debos::rotate(float FromX, float FromY, float ToX, float ToY) {
	Object *obj = data->getObject();
	if (obj) {
		if (obj->type == SPLINE) {
			SplineObject *so = (SplineObject*) obj;
			so->turnBezierPoint(FromX, FromY, ToX, ToY);
		}
	}
	float a, b, diff[2];
}

void Debos::scale(float FromX, float FromY, float ToX, float ToY) {
	Object *obj = data->getObject();
	if (obj) {
		if (obj->type == SPLINE) {
			SplineObject *so = (SplineObject*) obj;
			so->scaleBezierPoint(FromX, FromY, ToX, ToY);
		}
	}
}
