/*
 * debos.h
 *
 *  Created on: 22.08.2010
 *      Author: brainstar
 */

#ifndef DEBOS_H_
#define DEBOS_H_

#include <Qt/qwidget.h>
#include <QtGui/QtGui>
#include <QKeyEvent>
#include "tinyxml/tinyxml.h"
#include "document.h"
#include "gl_drawer.h"
using namespace std;

enum Mode {
	VIEW,
	SPLINE,
	LINE
};

class Debos : public QWidget {
Q_OBJECT;
public:
	Debos();
	~Debos();

public slots:
	void newFile();
	void loadFile();
	void saveFile();
	void exportFile();
	void closeFile();
	void aboutDebos();
	void activateViewMode();
	void activateSplineMode();
	void activateLineMode();

	void draw();

protected:
	void keyPressEvent( QKeyEvent * event );

signals:

private:
	Document* data;
	GLDrawer* gl;
	Mode mode;
};

#endif /* DEBOS_H_ */
