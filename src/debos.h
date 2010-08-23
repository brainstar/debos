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
#include "tinyxml/tinyxml.h"

class Debos : public QWidget {
Q_OBJECT;
public:
	Debos();
	~Debos();

public slots:
	void newFile();
	void loadFile();
	void saveFile();
	void closeFile();
	void aboutDebos();

signals:

private:
	TiXmlDocument* doc;
};

#endif /* DEBOS_H_ */
