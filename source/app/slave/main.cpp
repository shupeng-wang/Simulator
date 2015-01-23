/**
 * Simulator for relay
 */
#include <QApplication>
#include <QTranslator>
#include <QIcon>
#include <ctime>
#include "easylogging++.h"
#include "getopt.h"
#include "RelayDialog.h"

_INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
	_START_EASYLOGGINGPP(argc, argv);
	LINFO << "Start logging " << argv[0];
	
	QApplication app(argc, argv);
	RelayDialog *relay = new RelayDialog;
	relay->show();

	return app.exec();
}