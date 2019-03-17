/***************************************************************************
 *   Copyright (C) 2002-2003 Andi Peredri                                  *
 *   andi@ukr.net                                                          *
 *   Copyright (C) 2004-2007 Artur Wiebe                                   *
 *   wibix@gmx.de                                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include <QLibraryInfo>
#include <QStandardPaths>
//#include <QPlastiqueStyle>


#include "toplevel.h"
#include "common.h"


int main(int argc, char *argv[])
{
	QApplication app(argc,argv);
	QString current_lang = QLocale::system().name().split("_").front();
	QString qt_lang_path = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
	//QString qcheckers_share_path = PREFIX"/share/qcheckers/lang";
  QString lang_path = QStandardPaths::locate(QStandardPaths::DataLocation, "lang", QStandardPaths::LocateDirectory);

	qDebug()
		<< "Your Locale:" << current_lang << endl
		<< "QCheckers Translations path:" << lang_path << endl
		<< "QT Translations path:" << qt_lang_path << endl;

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
        lang_path)) {
    app.installTranslator(&app_tr);
  } else {
    qDebug() << "Loading QCheckers translations failed.";
  }

	myTopLevel* top = new myTopLevel();
	top->show();

	// command line
	if(app.arguments().length()==2) {
		top->open(app.arguments()[1]);
	}

	int exit = app.exec();

	delete top;
	return exit;
}
