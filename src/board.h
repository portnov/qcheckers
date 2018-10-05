/***************************************************************************
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

#ifndef _BOARD_H_
#define _BOARD_H_


#include <QFrame>

#include "theme.h"
#include "field.h"
#include "checkers.h"


class myBoard : public QFrame
{
	Q_OBJECT

public:
	myBoard(QWidget* parent);
	~myBoard();

  QSize sizeHint() const;
  void resizeEvent(QResizeEvent*);

	// returns coded move string: from_pos_string?to_pos_string
	QString doMove(int from_pos, int to_pos, bool white_player);
	// coded move.
	bool doMove(const QString& move, bool white_player);
	//
	void doFreeMove(int from, int to);

	void selectField(int field_num, bool is_on);

	void setTheme(const QString& theme_path, bool set_white);

	void setNotation(bool enabled, bool show_above);
	void setNotationFont(const QFont& f);

	void setColorWhite(bool);
	void reset();
	void adjustNotation(bool bottom_is_white);

	void setGame(int rules);

	bool whiteIsNext() const;

	int type() const { return m_game->type(); }
	// TODO
	const Checkers* game() const { return m_game; }

signals:
	void fieldClicked(int);

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void timerEvent(QTimerEvent*);

private:
	bool convert_move(const QString&, int* from, int* to);
	void fieldsSetup();
  void draw();
  void drawField(QPainter&, Field*, int, int);
  void invalidate();
  void beginSetup();
  void endSetup();
  int getTargetFieldSize(QSize size);

  void highlight(int, int, bool);

private:
	Field* m_fields[64];
  QPixmap* pixmap = NULL;
  bool pixmap_valid;

  Theme* m_theme;

  bool bottom_is_white;
  bool animation_in_progress;
  QColor m_src_color;
  QColor m_dst_color;
  Field* m_src_field;
  Field* m_dst_field;

  int animation_steps;
  int current_step;
  int animation_timer;
  bool animating_bottom_player;

	Checkers* m_game;
};


#endif

