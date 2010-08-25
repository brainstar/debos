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
	bool loadFile();
	bool loadFile(QString filename);
	void saveFile();
	void exportFile();
	void closeFile();
	void aboutDebos();
	void activateMode(Mode pMode);

	void draw();

	void mouseClick(float x, float y);

protected:
	void keyPressEvent(QKeyEvent *event);

signals:

private:
	void mouseClickView(float x, float y);
	void mouseClickSpline(float x, float y);
	void mouseClickLine(float x, float y);

	Document* data;
	GLDrawer* gl;
	Mode mode;
};

#endif /* DEBOS_H_ */
