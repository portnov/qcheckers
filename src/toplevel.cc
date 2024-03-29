// Version: $Id$
/***************************************************************************
 *   Copyright (C) 2002-2003 Andi Peredri                                  *
 *   andi@ukr.net                                                          *
 *   Copyright (C) 2004-2005 Artur Wiebe                                   *
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

#include <QLayout>
#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QFontDialog>
#include <QDebug>
#include <QStandardPaths>

#include "pdn.h"
#include "toplevel.h"
#include "newgamedlg.h"
#include "history.h"
#include "view.h"
#include "common.h"

#define CFG_THEME_PATH	CFG_KEY"ThemePath"
#define CFG_FILENAME	CFG_KEY"Filename"
#define CFG_KEEPDIALOG	CFG_KEY"ShowKeepDialog"
#define CFG_NOTATION	CFG_KEY"Notation"
#define CFG_NOT_ABOVE	CFG_KEY"NotationAbove"
#define CFG_NOT_FONT	CFG_KEY"NotationFont"
#define CFG_CLEAR_LOG	CFG_KEY"ClearLogOnNewRound"
#define CFG_GEOMETRY CFG_KEY"WindowGeometry"
#define CFG_STATE CFG_KEY"WindowState"


myTopLevel::myTopLevel()
{
	setWindowTitle(APPNAME);
	setWindowIcon(QIcon(":/icons/logo.svg"));

	m_newgame = new myNewGameDlg(this);
	make_central_widget();
  make_docks();
	make_actions();
	restore_settings();

	/*
	 * new computer game.
	 */
	m_view->newGame(m_newgame->rules(), m_newgame->freePlacement(),
			m_newgame->name(),
			m_newgame->isWhite(), m_newgame->opponent(),
			m_newgame->opponentName(), m_newgame->skill());

	if(layout()) {
		//layout()->setSizeConstraint(QLayout::SetFixedSize);
	}
}

void myTopLevel::make_docks() {
	m_history = new myHistory(this);

	connect(m_history, SIGNAL(previewGame(int)),
			m_view, SLOT(slot_preview_game(int)));
	connect(m_history, SIGNAL(applyMoves(const QString&)),
			m_view, SLOT(slot_apply_moves(const QString&)));
	connect(m_history, SIGNAL(newMode(bool, bool)),
			m_view, SLOT(slot_new_mode(bool, bool)));
	connect(m_view, SIGNAL(working(bool)),
			m_history, SLOT(slotWorking(bool)));

  m_history_dock = make_dock("history", tr("History"), Qt::RightDockWidgetArea, m_history);

	m_log = new QTextEdit(this);
    	//m_log->setFixedHeight(100); //FIXME
	m_log->setReadOnly(true);
  
  m_log_dock = make_dock("log", tr("Log"), Qt::BottomDockWidgetArea, m_log);

}

QDockWidget* myTopLevel::make_dock(const QString& name, const QString& title, Qt::DockWidgetArea area, QWidget* widget) {
  QDockWidget* dock = new QDockWidget(title, this);
  dock->setAllowedAreas(Qt::AllDockWidgetAreas);
  dock->setWidget(widget);
  dock->setObjectName(name);
  addDockWidget(area, dock);
  return dock;
}


void myTopLevel::make_actions()
{
  QString theme_path = QStandardPaths::locate(QStandardPaths::DataLocation, THEME_DIR, QStandardPaths::LocateDirectory);
  qDebug()
      << "Themes directory: " << theme_path << endl;
	// game menu actions
	gameNew = new QAction(QIcon(":/icons/logo.svg"), tr("&New..."), this);
	gameNew->setShortcut(tr("CTRL+N", "File|New"));
	connect(gameNew, SIGNAL(triggered()), this, SLOT(slot_new_game()));

	gameNextRound = new QAction(QIcon(":/icons/next.svg"),
			tr("&Next Round"), this);
	connect(gameNextRound, SIGNAL(triggered()),
			this, SLOT(slot_next_round()));

	gameStop = new QAction(QIcon(":/icons/stop.svg"), tr("&Stop"), this);
	connect(gameStop, SIGNAL(triggered()), m_view, SLOT(slotStopGame()));

	gameOpen = new QAction(QIcon(":/icons/fileopen.svg"), tr("&Open..."),
			this);
	gameOpen->setShortcut(tr("CTRL+O", "File|Open"));
	connect(gameOpen, SIGNAL(triggered()), this, SLOT(slot_open_game()));

	gameSave = new QAction(QIcon(":/icons/filesave.svg"), tr("&Save..."),
			this);
	gameSave->setShortcut(tr("CTRL+S", "File|Save"));
	connect(gameSave, SIGNAL(triggered()), this, SLOT(slot_save_game()));

	QAction* gameQuit = new QAction(QIcon(":/icons/exit.svg"), tr("&Quit"),
		this);
	gameQuit->setShortcut(tr("CTRL+Q", "File|Quit"));
	connect(gameQuit, SIGNAL(triggered()),
		this, SLOT(close()));
//		QApplication::instance(), SLOT(quit()));

	// view menu actions
	viewNotation = new QAction(tr("&Show Notation"), this);
	viewNotation->setCheckable(true);
	connect(viewNotation, SIGNAL(toggled(bool)),
		this, SLOT(slot_notation(bool)));

	viewNotationAbove = new QAction(tr("Show notation &above men"), this);
	viewNotationAbove->setCheckable(true);
	connect(viewNotationAbove, SIGNAL(toggled(bool)),
		this, SLOT(slot_notation(bool)));

	// settings menu
	settingsKeep = new QAction(tr("&Confirm aborting current game"), this);
	settingsKeep->setCheckable(true);
	//
	settingsClearLog = new QAction(tr("Clear &log on new round"), this);
	settingsClearLog->setCheckable(true);
	connect(settingsClearLog, SIGNAL(toggled(bool)),
		m_view, SLOT(slotClearLog(bool)));
	//
	QAction* settingsNotationFont = new QAction(tr("&Notation font..."),
			this);
	connect(settingsNotationFont, SIGNAL(triggered()),
		this, SLOT(slot_notation_font()));

	// help menu actions
	QAction* helpRules = new QAction(tr("&Rules of Play"), this);
	helpRules->setShortcut(tr("F1", "Help|Help"));
	connect(helpRules, SIGNAL(triggered()), this, SLOT(slot_help()));

	QAction* helpAbout = new QAction(QIcon(":/icons/logo.svg"),
		tr("&About")+" "APPNAME, this);
	connect(helpAbout, SIGNAL(triggered()), this, SLOT(slot_about()));

	QAction* helpAboutQt = new QAction(tr("About &Qt"), this);
	connect(helpAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));


	/*
	 * toolbar
	 */
	QToolBar* tb = addToolBar(tr("&Toolbar"));
	tb->setMovable(false);
	tb->addAction(gameNew);
	tb->addAction(gameOpen);
	tb->addAction(gameSave);
	tb->addSeparator();
	tb->addAction(gameNextRound);
	tb->addAction(gameStop);


	/*
	 * menus
	 */
	QMenu* gameMenu = menuBar()->addMenu(myTopLevel::tr("&Game"));
	gameMenu->addAction(gameNew);
	gameMenu->addAction(gameOpen);
	gameMenu->addAction(gameSave);
	gameMenu->addSeparator();
	gameMenu->addAction(gameNextRound);
	gameMenu->addAction(gameStop);
	gameMenu->addSeparator();
	gameMenu->addAction(gameQuit);

	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(tb->toggleViewAction());
	viewMenu->addSeparator();
	viewMenu->addAction(viewNotation);
	viewMenu->addAction(viewNotationAbove);
  viewMenu->addAction(m_history_dock->toggleViewAction());
  viewMenu->addAction(m_log_dock->toggleViewAction());

	QMenu* settingsMenu = menuBar()->addMenu(tr("&Settings"));
	settingsMenu->addAction(settingsKeep);
	settingsMenu->addSeparator();
	settingsMenu->addAction(settingsNotationFont);
	settingsMenu->addSeparator();
	settingsMenu->addAction(settingsClearLog);

	QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(helpRules);
	helpMenu->addSeparator();
	helpMenu->addAction(helpAbout);
	helpMenu->addAction(helpAboutQt);


	/*
	 * THEMES. create a default theme.
	 */
	QActionGroup* themes_grp = new QActionGroup(this);
	themes_grp->setExclusive(true);

	QAction* default_theme = new QAction(tr(DEFAULT_THEME), themes_grp);
	default_theme->setCheckable(true);
	m_themes[default_theme] = DEFAULT_THEME;
	default_theme->setChecked(true);
	set_theme(default_theme);//FIXME TODO
	themes_grp->addAction(default_theme);

	viewMenu->addSeparator();
	viewMenu->addAction(default_theme);
	read_themes(themes_grp,
			viewMenu,QDir::homePath()+"/"USER_PATH"/"THEME_DIR);
	//TODO-hardcoded
	read_themes(themes_grp,
			viewMenu, theme_path);

	connect(themes_grp, SIGNAL(triggered(QAction*)),
		this, SLOT(set_theme(QAction*)));
}


void myTopLevel::read_themes(QActionGroup* menu_grp, QMenu* menu, 
	const QString& path)
{
	QDir sharedir(path);
	QStringList themes = sharedir.entryList(QDir::Dirs|QDir::Readable);
	themes.removeAll(".");
	themes.removeAll("..");

	for(QStringList::iterator it=themes.begin(); it!=themes.end(); ++it) {
		sharedir.cd(*it);

		QString theme_dir = sharedir.absolutePath();
    qInfo("Check: %s", qUtf8Printable(theme_dir));
    Theme* theme = new Theme(this, theme_dir, 0);
    if (! theme->isValid()) {
      qWarning() << "Theme in "
				<< sharedir.absolutePath() << " is invalid";
    } else {
			QString themename = theme->name();

			QAction* action = new QAction(themename, this);
			menu_grp->addAction(action);
			menu->addAction(action);
			action->setCheckable(true);
			m_themes[action] = theme_dir;
		}

		sharedir.cdUp();
	}
}


void myTopLevel::restore_settings()
{
	QSettings cfg(APPNAME, APPNAME);

	QString theme_path = cfg.value(CFG_THEME_PATH, DEFAULT_THEME)
		.toString();
	for(myThemeMap::iterator it = m_themes.begin(); it!=m_themes.end();
			it++) {
		if(it.value() == theme_path) {
			it.key()->setChecked(true);
			set_theme(it.key());
			break;
		}
	}

	filename = cfg.value(CFG_FILENAME).toString();

	viewNotation->setChecked(cfg.value(CFG_NOTATION, false).toBool());
	viewNotationAbove->setChecked(cfg.value(CFG_NOT_ABOVE, true).toBool());
	slot_notation(true);	// here: arg is ignored by the function

	bool clear_log = cfg.value(CFG_CLEAR_LOG, true).toBool();
	settingsClearLog->setChecked(clear_log);
	m_view->slotClearLog(clear_log);

	//
	settingsKeep->setChecked(cfg.value(CFG_KEEPDIALOG, true).toBool());

	QFont cfont;
	if(cfont.fromString(cfg.value(CFG_NOT_FONT, "").toString()))
	m_view->setNotationFont(cfont);

	// new game
	m_newgame->readSettings(&cfg);

  restoreGeometry(cfg.value(CFG_GEOMETRY).toByteArray());
  restoreState(cfg.value(CFG_STATE).toByteArray());
}


void myTopLevel::make_central_widget()
{
	m_view = new myView(this);

	connect(m_view, SIGNAL(working(bool)),
			this, SLOT(slot_working(bool)));

	setCentralWidget(m_view);
}


void myTopLevel::warning(const QString& text)
{
	QMessageBox::warning(this, tr("Error")+" - "APPNAME, text);
}


void myTopLevel::information(const QString& caption, const QString& text)
{
	QDialog* dlg = new QDialog(this);
	dlg->setModal(true);
	dlg->setWindowTitle(caption+" - "APPNAME);

	// logo left
	QLabel* logo = new QLabel(dlg);
	logo->setPixmap(QPixmap(":/icons/dialog.png"));

	// text editor
	QTextEdit* te = new QTextEdit(text, dlg);
	te->setReadOnly(true);
	te->setMinimumWidth(m_view->width()-100);
	te->setMinimumHeight(m_view->height()-200);

	// close button
	QPushButton* button = new QPushButton(tr("&Close"), dlg);
	connect(button, SIGNAL(clicked()), dlg, SLOT(accept()));

	// Layout.
	QHBoxLayout* hb = new QHBoxLayout();
	hb->addWidget(logo, 0, Qt::AlignTop);
	hb->addWidget(te, 1);

	QHBoxLayout* hb_button = new QHBoxLayout();
	hb_button->addStretch();
	hb_button->addWidget(button);

	QVBoxLayout* vb = new QVBoxLayout(dlg);
	vb->addLayout(hb, 1);
	vb->addSpacing(5);
	vb->addLayout(hb_button);

	// go
	dlg->exec();
	delete dlg;
}

myHistory* myTopLevel::get_history() const {
  return m_history;
}

void myTopLevel::clear_log() {
  m_log->clear();
}

void myTopLevel::append_log(const QString& text) {
  m_log->append(text);
	m_log->ensureCursorVisible();
}

void myTopLevel::slot_save_game()
{
	QString fn = QFileDialog::getSaveFileName(this,
		tr("Save Game")+" - "APPNAME, filename, "PDN Files (*."EXT")");
	if(!fn.isEmpty()) {
		if(fn.right(3)!=EXT) {
			fn += "."EXT;
		}

		if(m_view->savePdn(fn)) {
			filename = fn;
		} else {
			warning(tr("Could not save: ")+filename);
		}
	}
}


void myTopLevel::slot_open_game()
{
	QString fn = QFileDialog::getOpenFileName(this,
		tr("Open Game")+" - "APPNAME, filename, "PDN Files (*."EXT")");
	if(!fn.isEmpty()) {
		open(fn);
	}
}


void myTopLevel::open(const QString& fn)
{
	if(m_view->openPdn(fn)) {
		filename = fn;
	}
}


void myTopLevel::closeEvent(QCloseEvent* e)
{
	if(!keep_game()) {
		store_settings();
		e->accept();
	} else {
		e->ignore();
	}
}


void myTopLevel::store_settings()
{
	QSettings config(APPNAME, APPNAME);

	for(myThemeMap::iterator it=m_themes.begin(); it!=m_themes.end();
			it++) {
		if(it.key()->isChecked()) {
			config.setValue(CFG_THEME_PATH, it.value());
			break;
		}
	}

	config.setValue(CFG_FILENAME, filename);

	config.setValue(CFG_NOTATION, viewNotation->isChecked());
	config.setValue(CFG_NOT_ABOVE, viewNotationAbove->isChecked());

	config.setValue(CFG_KEEPDIALOG, settingsKeep->isChecked());
	config.setValue(CFG_NOT_FONT, m_view->notationFont().toString());
	config.setValue(CFG_CLEAR_LOG, settingsClearLog->isChecked());

	// new game
	m_newgame->writeSettings(&config);

  config.setValue(CFG_GEOMETRY, saveGeometry());
  config.setValue(CFG_STATE, saveState());
}


void myTopLevel::set_theme(QAction* action)
{
	QString path = m_themes[action];
	m_view->setTheme(path);
}


void myTopLevel::slot_help()
{
	QString text = 
		tr("<p>In the beginning of game you have 12 checkers (men). "
		"The men move forward only. The men can capture:"
		"<ul>"
		"<li>by jumping forward only (english rules);"
		"<li>by jumping forward or backward (russian rules)."
		"</ul>"
		"<p>A man which reaches the far side of the board becomes a king. "
		"The kings move forward or backward:"
		"<ul>"
		"<li>to one square only (english rules);"
		"<li>to any number of squares (russian rules)."
		"</ul>"
		"<p>The kings capture by jumping forward or backward. "
		"Whenever a player is able to make a capture he must do so.");

	information(tr("Rules of Play"), text);
}


void myTopLevel::slot_about()
{
	QString text =
		APPNAME", a board game.<br>Version "VERSION"<br><br>"
		COPYRIGHT"<br>"
		  	"<a href=\""HOMEPAGE"\">"HOMEPAGE"</a><br><br>"
		"Contributors:<br>"CONTRIBS"<br><br>"
		"This program is distributed under the terms "
		"of the GNU General Public License Version 2.";

	information(tr("About"), text);
}


void myTopLevel::slot_new_game()
{
	if(keep_game())
		return;

	if(m_newgame->exec()==QDialog::Accepted) {
		m_view->newGame(m_newgame->rules(), m_newgame->freePlacement(),
				m_newgame->name(),
				m_newgame->isWhite(), m_newgame->opponent(),
				m_newgame->opponentName(), m_newgame->skill());
	}
}


void myTopLevel::slot_working(bool working)
{
	bool disable = !working;

	gameNew->setEnabled(disable);
	gameSave->setEnabled(disable);

	gameNextRound->setEnabled(disable);// FIXME !m_view->isAborted());
	gameOpen->setEnabled(disable);
	gameStop->setEnabled(!disable);

	m_view->setEnabled(disable);
}


bool myTopLevel::keep_game()
{
	if(!settingsKeep->isChecked() || m_view->isAborted()) {
		return false;
	}

	int answer = QMessageBox::question(this, tr("Abort Game?")+" - "APPNAME,
		tr("Current game will be lost if you continue.\n"
		"Do you really want to discard it?"),
		QMessageBox::Yes, QMessageBox::No);

	if(answer == QMessageBox::Yes) {
		return false;
	}

	return true;
}


void myTopLevel::slot_next_round()
{
	if(!keep_game()) {
		m_view->slotNextRound();
	}
}


void myTopLevel::slot_notation_font()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, m_view->notationFont(), this);
	if(ok) {
		m_view->setNotationFont(font);
	}
}


void myTopLevel::slot_notation(bool)
{
	m_view->setNotation(viewNotation->isChecked(),
			viewNotationAbove->isChecked());
}

