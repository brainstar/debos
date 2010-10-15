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
	EDIT
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
	void mouseMove(int x, int y);

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);

signals:

private:
	void mouseClickView(float x, float y);
	void mouseClickEdit(float x, float y);

	Document* data;
	GLDrawer* gl;
	Mode mode;
	
	bool bGrab;
	int iMouse[2];
};

#endif /* DEBOS_H_ */
