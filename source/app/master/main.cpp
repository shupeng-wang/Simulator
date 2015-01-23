/**
 * Simulator for relay
 */
#include <QApplication>
#include <QTranslator>
#include <QIcon>
#include <ctime>
#include "easylogging++.h"
#include "getopt.h"
#include "MasterDialog.h"


_INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
	_START_EASYLOGGINGPP(argc, argv);
	LINFO << "Start logging " << argv[0];
	
	QApplication app(argc, argv);
	MasterDialog *dialog = new MasterDialog;
	if ( !dialog->init() ) {
		return 1;
	}
	dialog->show();

	return app.exec();
}