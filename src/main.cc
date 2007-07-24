#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include <QLibraryInfo>
//#include <QPlastiqueStyle>


#include "toplevel.h"
#include "common.h"


int main(int argc, char *argv[])
{
	QApplication app(argc,argv);
	QString current_lang = QLocale::system().name().split("_").front();
	QString qt_lang_path = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
	QString qcheckers_share_path = PREFIX"/share/qcheckers";

	qDebug()
		<< "Your Locale:" << current_lang << endl
		<< "QCheckers Prefix path:" << PREFIX << endl
		<< "QT Translations path:" << qt_lang_path << endl;

	if(current_lang!="en") {
		// Qt translations
		QTranslator qt_tr;
		if(qt_tr.load("qt_" + current_lang, qt_lang_path)) {
			app.installTranslator(&qt_tr);
		} else {
			qDebug() << "Loading Qt translations failed.";
		}

		// App translations
		QTranslator app_tr;
		if(app_tr.load("qcheckers_" + current_lang,
					qcheckers_share_path)) {
			app.installTranslator(&app_tr);
		} else {
			qDebug() << "Loading KCheckers translations failed.";
		}
	}

	myTopLevel* top = new myTopLevel();
	top->show();

	// command line
	if(app.argc()==2) {
		top->open(app.argv()[1]);
	}

	int exit = app.exec();

	delete top;
	return exit;
}
