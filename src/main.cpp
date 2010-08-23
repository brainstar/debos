/*
 * main.cpp
 *
 *  Created on: 22.08.2010
 *      Author: brainstar
 */

#include "debos.h"
using namespace std;

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	Debos* debos = new Debos();
	debos->show();

	return app.exec();
}
