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
	bView = true;
	edit = NONE;

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
	
	statusbarMode = new QStatusBar();
	statusbarEdit = new QStatusBar();
	statusbarAction = new QStatusBar();
	
	QHBoxLayout* lay = new QHBoxLayout();
	
	lay->addWidget(statusbarMode, 1);
	lay->addWidget(statusbarEdit, 2);
	lay->addWidget(statusbarAction, 2);
	
	QVBoxLayout* layout = new QVBoxLayout();
	
	layout->addWidget(menubar);
	layout->addStretch(0.5);
	layout->addWidget(gl, 1);
	layout->addStretch(0.5);
	layout->addLayout(lay);
	layout->setSpacing(0);
	layout->setMargin(0);

	this->setLayout(layout);

	this->setWindowTitle("debos");
	
	timerAction = new QTimer();
	timerAction->setSingleShot(true);

	// Loading default file
	data = 0;

	if (!loadFile("test.xml")) {
		qDebug("Going to create a new file");
		newFile();
	}

	connect(gl, SIGNAL(mouseClicked(float, float)), this, SLOT(mouseClick(float, float)));
	connect(gl, SIGNAL(mouseMoved(int, int)), this, SLOT(mouseMove(int, int)));
	connect(gl, SIGNAL(draw()), this, SLOT(draw()));
	connect(timerAction, SIGNAL(timeout()), this, SLOT(clearActionBar()));

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
	statusbarMode->showMessage("View Mode");
	setActionBar("New File created");
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
			statusbarMode->showMessage("View Mode");
			setActionBar("File loaded");
			qDebug("Loading successful");
			return true;
		}
		else {
			delete doc;
			statusbarMode->clearMessage();
			setActionBar("Loading File not possible");
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
		setActionBar("File exported");
	}
}

void Debos::saveFile() {
	// Save file to *.xml file
	QString filename = QFileDialog::getSaveFileName(this, "Save File", "test.xml", "XML Files (*.xml)");
	if (!filename.isEmpty()) {
		data->save(filename.toStdString());
		setActionBar("File saved");
	}
}

void Debos::closeFile() {
	// Close file without saving
	gl->hide();
	if (data) {
		delete data;
		qDebug("Closing file");
		setActionBar("File closed");
	}
	gl->data = data = 0;
	statusbarMode->clearMessage();
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
		if (edit == GRAB) {
			float x, y;
			gl->ptc(ix, iy, &x, &y);
			grab(iMouse[0], iMouse[1], x, y);
		}
		else if (edit == ROTATE) {
			float x, y;
			gl->ptc(ix, iy, &x, &y);
			rotate(iMouse[0], iMouse[1], x, y);
		}
		else if (edit == SCALE) {
			float x, y;
			gl->ptc(ix, iy, &x, &y);
			scale(iPos[0], iPos[1], x, y);
		}
		gl->updateGL();
	}
	gl->ptc(ix, iy, &iMouse[0], &iMouse[1]);
}

void Debos::clearActionBar() {
	statusbarAction->clearMessage();
}

void Debos::setActionBar(QString message) {
	timerAction->stop();
	statusbarAction->showMessage(message);
	timerAction->start(5000);
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
			if(event->key() == Qt::Key_V) {
				editAbort();
				activateMode(VIEW);
				statusbarMode->showMessage("View Mode");
			}
			else if(event->key() == Qt::Key_E) {
				activateMode(EDIT);
				statusbarMode->showMessage("Edit Mode");
			}

			// Modification of Viewport allowed
			if (bView) {
				// Move camera
				if (event->key() == Qt::Key_Left) { viewMoveLeft(); }
				else if (event->key() == Qt::Key_Right) { viewMoveRight(); }
				else if (event->key() == Qt::Key_Down) { viewMoveDown(); }
				else if (event->key() == Qt::Key_Up) { viewMoveUp(); }
				else if (event->key() == Qt::Key_Plus) { viewZoomIn(); }
				else if (event->key() == Qt::Key_Minus) { viewZoomOut(); }
			}
		}

		// View mode is active
		else if (mode == VIEW) {
			if (event->key() == Qt::Key_C) { closeFile(); }
			// Select previous Object
			else if (event->key() == Qt::Key_Down) { data->prevObject(); }
			// Select next Object
			else if (event->key() == Qt::Key_Up) { data->nextObject(); }
			// Add a SplineObject
			else if (event->key() == Qt::Key_S) {
				data->addSplineObject();
				setActionBar("Added Spline-Object");
			}
			// Delete an Object
			else if (event->key() == Qt::Key_X) {
				if (data->deleteObject()) { setActionBar("Object deleted"); }
			}
		}
		
		// Edit mode is active
		else if (mode == EDIT) {
			if (event->key() == Qt::Key_Escape) {
				editAbort();
				statusbarEdit->clearMessage();
			}
			// Alt Modifier is used for moving operations
			else if (event->modifiers() & Qt::AltModifier) {
				// Move BezierPoint
				if (event->key() == Qt::Key_Down) { editMoveBezierPoint(0., -1.); }
				else if (event->key() == Qt::Key_Up) { editMoveBezierPoint(0., 1.); }
				else if (event->key() == Qt::Key_Left) { editMoveBezierPoint(-1., 0.); }
				else if (event->key() == Qt::Key_Right) { editMoveBezierPoint(1., 0.); }
			}
			// No Modifier
			else {				
				// Select previous/next BezierPoint
				if (event->key() == Qt::Key_Down) { editChangeBezierPoint(1); }
				else if (event->key() == Qt::Key_Up) { editChangeBezierPoint(-1); }
				// Rotate BezierPoint
				else if (event->key() == Qt::Key_Left) { editRotateBezierPoint(1); }
				else if (event->key() == Qt::Key_Right) { editRotateBezierPoint(-1); }	
				// Delete BezierPoint
				else if (event->key() == Qt::Key_Delete) { editDeleteBezierPoint(); }
			}
			
			// Edit-Buttons allowed
			if (edit == NONE) {
				// Move BezierPoint
				if (event->key() == Qt::Key_G) { editStartGrab(); }
				// Rotate BezierPoint
				else if (event->key() == Qt::Key_R) { editStartRotate(); }
				// Scale BezierPoint
				else if (event->key() == Qt::Key_S) { editStartScale(); }
				// Close Bezier-Curve
				else if (event->key() == Qt::Key_C) { editCloseCurve(); }
				// Add BezierPoints
				else if (event->key() == Qt::Key_A) { editStartAdd(); }
			}
		}
	}

	QWidget::keyPressEvent(event); // important, default key handling
	gl->updateGL();
}

void Debos::activateMode(Mode pMode) {
	if(mode != pMode) {
		mode = pMode;
	}
}

void Debos::mouseClickView(float x, float y) {
}

void Debos::mouseClickEdit(float x, float y) {
	if (edit == ADD) {
		Object *obj;
		if (obj = data->getObject()) {
			obj->addPoint(x, y, 0.0);
			gl->updateGL();
		}
	}
	else if (edit != NONE) {
		edit = NONE;
		bView = true;
		statusbarEdit->clearMessage();
	}
}

bool Debos::viewMoveUp() {
	float *g = data->getGrid();
	if (!g) return false;
	
	float h = (*(g+3)) - (*(g+2));
	*(g + 2) += h / 20.0;
	*(g + 3) += h / 20.0;
	gl->simResize();
	
	return true;
}

bool Debos::viewMoveDown() {
	float *g = data->getGrid();
	if (!g) return false;
	
	float h = (*(g+3)) - (*(g+2));
	*(g + 2) -= h / 20.0;
	*(g + 3) -= h / 20.0;
	gl->simResize();
	
	return true;
}

bool Debos::viewMoveLeft() {
	float *g = data->getGrid();
	if (!g) return false;
	
	float w = (*(g+1)) - (*g);
	*g -= w / 20.0;
	*(g + 1) -= w / 20.0;
	gl->simResize();
	
	return true;
}

bool Debos::viewMoveRight() {
	float *g = data->getGrid();
	if (!g) return false;
	
	float w = (*(g+1)) - (*g);
	*g += w / 20.0;
	*(g + 1) += w / 20.0;
	gl->simResize();
	
	return true;
}

bool Debos::viewZoomIn() {
	float *g = data->getGrid();
	if (!g) return false;
	
	float w = (*(g+1)) - (*g);
	float h = (*(g+3)) - (*(g+2));
	*g += w / 40.0;
	*(g+1) -= w / 40.0;
	*(g+2) += h / 40.0;
	*(g+3) -= h / 40.0;
	gl->simResize();
	
	return true;
}

bool Debos::viewZoomOut() {
	float *g = data->getGrid();
	if (!g) return false;
	
	float w = (*(g+1)) - (*g);	
	float h = (*(g+3)) - (*(g+2));
	*g -= w / 40.0;
	*(g+1) += w / 40.0;
	*(g+2) -= h / 40.0;
	*(g+3) += h / 40.0;
	gl->simResize();
	
	return true;
}

bool Debos::editAbort() {
	if (edit == GRAB) {
		grab(iMouse[0], iMouse[1], iPos[0], iPos[1]);
		edit = NONE;
		setActionBar("Aborted grabbing");
	}
	else if (edit == ROTATE) {
		rotate(iMouse[0], iMouse[1], iPos[0], iPos[1]);
		edit = NONE;
		bView = true;
		setActionBar("Aborted rotating");
	}
	else if (edit == SCALE) {
		Object *obj = data->getObject();
		if (obj) {
			if (obj->type == SPLINE) {
				SplineObject *so = (SplineObject*) obj;
				so->recoverBezierPoint();
			}
		}
		edit = NONE;
		bView = true;
		setActionBar("Aborted scaling");
	}
	else if (edit == ADD) {
		edit = NONE;
	}
}

bool Debos::editMoveBezierPoint(float x, float y) {
	Object *obj = data->getObject();
	if (obj) {
		if (obj->type == SPLINE)
			((SplineObject*)obj)->moveBezierPoint(x, y);
			return true;
	}
	return false;
}

bool Debos::editStartGrab() {
	Object *obj = data->getObject();
	if (obj) {
		if (obj->type == SPLINE) {
			SplineObject *so = (SplineObject*) obj;
			if (so->beziers.begin() != so->beziers.end()) {
				edit = GRAB;
				statusbarEdit->showMessage("Grab");
				iPos[0] = iMouse[0];
				iPos[1] = iMouse[1];
				return true;
			}
		}
	}
	return false;
}

bool Debos::editStartRotate() {
	Object *obj = data->getObject();
	if (obj) {
		if (obj->type == SPLINE) {
			SplineObject *so = (SplineObject*) obj;
			if (so->beziers.begin() != so->beziers.end()) {
				edit = ROTATE;
				statusbarEdit->showMessage("Rotate");
				bView = false;
				iPos[0] = iMouse[0];
				iPos[1] = iMouse[1];
				return true;
			}
		}
	}
	return false;
}

bool Debos::editStartScale() {
	Object *obj = data->getObject();
	if (obj) {
		if (obj->type == SPLINE) {
			SplineObject *so = (SplineObject*) obj;
			if (so->beziers.begin() != so->beziers.end()) {
				so->saveBezierPoint();
				edit = SCALE;
				statusbarEdit->showMessage("Scale");
				bView = false;
				iPos[0] = iMouse[0];
				iPos[1] = iMouse[1];
				return true;
			}
		}
	}
	return false;
}

bool Debos::editCloseCurve() {
	Object *obj = data->getObject();
	if (obj) {
		if (obj->type == SPLINE) {
			SplineObject *so = (SplineObject*) obj;
			so->bClosed = !(so->bClosed);
			so->computeSplines();
			return true;
		}
	}
	return false;
}

bool Debos::editStartAdd() {
	if (data->getObject()) {
		edit = ADD;
		statusbarEdit->showMessage("Add");
	}
}

bool Debos::editChangeBezierPoint(int direction) {
	Object *obj = data->getObject();
	if (obj) {
		if (direction > 0) { return obj->nextInstance(); }
		else { return obj->prevInstance(); }
	}
	return false;
}

bool Debos::editRotateBezierPoint(int direction) {
	if (Object *obj = data->getObject()) {
		if (obj->type == SPLINE)
			((SplineObject*)obj)->turnBezierPoint(10. * direction);
	}
}

bool Debos::editDeleteBezierPoint() {
	Object *obj = data->getObject();
	if (obj) {
		obj->deleteInstance();
		gl->updateGL();
		return true;
	}
	return false;
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
