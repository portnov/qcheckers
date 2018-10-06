// version: $Id$
/***************************************************************************
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
#include <QLayout>
#include <QtGui>
#include <QDebug>

#include "board.h"
#include "common.h"
#include "pdn.h"
#include "echeckers.h"
#include "rcheckers.h"
/*
#include "newgamedlg.h"

#include "player.h"
#include "humanplayer.h"
#include "computerplayer.h"
*/

QColor myColorInterpolator(const QColor &start, const QColor &end, qreal progress) {
    qreal r = (1 - progress) * start.red() + progress * end.red();
    qreal g = (1 - progress) * start.green() + progress * end.green();
    qreal b = (1 - progress) * start.blue() + progress * end.blue();
    //qDebug("from %s to %s, progress %f", qUtf8Printable(start.name()), qUtf8Printable(end.name()), progress);
    return QColor(r, g, b);
}

myBoard::myBoard(QWidget* parent)
	: QFrame(parent)
{
	/*
	 * board & info 
	 */
	setFrameStyle(QFrame::Box|QFrame::Plain);
	for(int i=0; i<64; i++) {
		m_fields[i] = new Field(this, i);
	}

  pixmap_valid = false;
  m_src_field = NULL;
  m_dst_field = NULL;
  animation_steps = 10;
  current_step = 0;
  animation_timer = startTimer(100);

// 	for(int i=0; i<32; i++) {
// 		connect(m_fields[i], SIGNAL(click(int)),
// 				this, SIGNAL(fieldClicked(int)));
// 	}


	/*
	 * game init
	 */
	m_game = 0;

}


myBoard::~myBoard()
{
  for (int i = 0; i < 64; i++) {
    delete m_fields[i];
  }
	if(m_game) {
		delete m_game;
	}
  if (pixmap) {
    delete pixmap;
  }
}

void myBoard::beginSetup() {
  for (int i = 0; i < 64; i++) {
    m_fields[i]->beginSetup();
  }
}

void myBoard::endSetup() {
  for (int i = 0; i < 64; i++) {
    m_fields[i]->endSetup();
  }
}

void myBoard::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    draw();
    Q_ASSERT(pixmap_valid);
    p.drawPixmap(0, 0, *pixmap);

    p.end();
}

void myBoard::setTheme(const QString& path, bool set_white)
{
  int fieldSize = getTargetFieldSize(size());
  m_theme = new Theme(this, path, 0);
  if (fieldSize > 3) {
    if (m_theme->getIsResizeable()) {
      m_theme->setTargetSize(fieldSize);
    }
  }
  for (int i = 0; i < 64; i++) {
    m_fields[i]->setTheme(m_theme);
  }

  beginSetup();

	setColorWhite(set_white);

	for(int i=0; i<32; i++) {
		m_fields[i]->setPattern(2);
	}
	for(int i=32; i<64; i++) {
		m_fields[i]->setPattern(1);
	}

  //setFixedSize(sizeHint());
  if (! m_theme->getIsResizeable()) {
    int width = m_theme->getFieldWidth()*8 + 2*frameWidth();
    int height = m_theme->getFieldHeight()*8 + 2*frameWidth();
    setFixedSize(width, height);
  } else {
    setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  }

	if(m_game) {
		fieldsSetup();
	}

  endSetup();
  repaint();
}

QSize myBoard::sizeHint() const {
  if (! m_theme) {
    return QSize();
  } else {
    int width = m_theme->getFieldWidth()*8 + 2*frameWidth();
    int height = m_theme->getFieldHeight()*8 + 2*frameWidth();
    return QSize(width, height);
    //qInfo("Size is fixed: %d x %d", width, height);
  }
}

void myBoard::reset()
{
	int new_board[32];

	for(int i=0; i<12; i++) {
		new_board[i]=MAN2;
	}
	for(int i=12; i<20; i++) {
		new_board[i]=FREE;
	}
	for(int i=20; i<32; i++) {
		new_board[i]=MAN1;
	}

	// reset frames.
	for(int i=0; i<32; i++) {
		m_fields[i]->showFrame(false);
	}

	if(m_game) {
		m_game->setup(new_board);
	}

	fieldsSetup();
}


void myBoard::adjustNotation(bool bottom_is_white)
{
	if(!m_game) {
		return;
	}

  for(int i=0; i<32; i++) {
    m_fields[i]->setLabel(m_game->getFieldNotation(i, bottom_is_white));
  }
  invalidate();
  repaint();
}


void myBoard::fieldsSetup()
{
	for(int i=0; i<32; i++) {
    m_fields[i]->set(m_game->item(i), bottom_is_white);
	}
  invalidate();
}

void myBoard::invalidate() {
  pixmap_valid = false;
  for (int i = 0; i < 64;  i++) {
    m_fields[i]->invalidate();
  }
}

void myBoard::drawField(QPainter& painter, Field* field, int row, int column) {
  int width = size().width();
  int height = size().height();

  int rowHeight = height / 8;
  int colWidth = width / 8;
  int size = rowHeight < colWidth ? rowHeight : colWidth;

  QRect rect(column * size, row * size, size, size);
  field->draw(painter, rect);
}

void myBoard::draw() {
  if (pixmap_valid) {
    return;
  }
  QPixmap* prev = pixmap;
  pixmap = new QPixmap(size());
  pixmap->fill(Qt::white);
  QPainter painter(pixmap);
	for(int i=0; i<4; i++) {
		for(int k=0; k<4; k++) {
			drawField(painter, m_fields[i*8+k+32], i*2,  k*2  );
			drawField(painter, m_fields[i*8+k   ], i*2,  k*2+1);
			drawField(painter, m_fields[i*8+k+4 ], i*2+1,k*2  );
			drawField(painter, m_fields[i*8+k+36], i*2+1,k*2+1);
		}
	}

  if (animation_in_progress && m_src_field && m_dst_field) {
    painter.setPen(QPen(m_src_color, 5));
    painter.drawRect(m_src_field->rect());
    painter.setPen(Qt::black);

    painter.setPen(QPen(m_dst_color, 5));
    painter.drawRect(m_dst_field->rect());
    painter.setPen(Qt::black);
  }
  painter.end();
  update();
  pixmap_valid = true;
  delete prev;
}

void myBoard::setColorWhite(bool b)
{
  bottom_is_white = b;
}

void myBoard::setNotation(bool s, bool above)
{
	for(int i=0; i<32; i++) {
		m_fields[i]->showLabel(s, above);
	}
  invalidate();
  repaint();
}

/*
void myBoard::do_move(const QString& move)
{
	qDebug() << __PRETTY_FUNCTION__;
	if(!m_current->isHuman()) {
	add_log(myBoard::Warning, tr("It's not your turn."));
	return;
	}

	int from_num, to_num;
	if(extract_move(move, &from_num, &to_num)) {
	slot_click(from_num);
		slot_click(to_num);
	} else
	add_log(myBoard::Warning, tr("Syntax error. Usage: /from-to"));
}
	*/



bool myBoard::convert_move(const QString& move_orig, int* from_num, int* to_num)
{
	QString move = move_orig.toUpper().replace('X', '-');
	QString from;
	QString to;
	int sect = move.count('-');

	*from_num = *to_num = -1;

	from = move.section('-', 0, 0);
	to = move.section('-', sect, sect);

	if(from!=QString::null && to!=QString::null) {
		for(int i=0; i<32; i++) {
			if(m_fields[i]->label()==from) {
				*from_num = m_fields[i]->number();
			}
			if(m_fields[i]->label()==to) {
				*to_num = m_fields[i]->number();
			}
		}

		if(*from_num>=0 && *to_num>=0) {
			return true;
		}
	}

	return false;
}


void myBoard::setNotationFont(const QFont& f)
{
	setFont(f);
	for(int i=0; i<32; i++) {
		m_fields[i]->fontUpdate();
	}
}


void myBoard::setGame(int rules)
{
	if(m_game) {
		delete m_game;
	}

	if(rules==ENGLISH) {
		m_game = new ECheckers();
	} else {
		m_game = new RCheckers();
	}

	reset();
}


void myBoard::selectField(int field_num, bool is_on)
{
	for(int i=0; i<32; i++) {
		if(i==field_num) {
			m_fields[i]->showFrame(is_on);
		} else {
			m_fields[i]->showFrame(false);
		}
	}
}


QString myBoard::doMove(int from_num, int to_num, bool white_player)
{
	bool bottom_player = (white_player && bottom_is_white)
		|| (!white_player && !bottom_is_white);

	int from_pos = from_num;
	int to_pos = to_num;

	if(!bottom_player) {
		from_pos = 31-from_pos;
		to_pos = 31-to_pos;
		m_game->fromString(m_game->toString(true));
	}
	if(!m_game->go1(from_pos, to_pos)) {
		return QString::null;
		/*
		qDebug() << __PRETTY_FUNCTION__
			<< from_pos << "," << to_pos
			<< " could not move.";
		*/
	}
	if(!bottom_player) {
		m_game->fromString(m_game->toString(true));
	}

	fieldsSetup();
  highlight(from_num, to_num, bottom_player);

	return QString("%1?%3")
		.arg(m_fields[from_num]->label())
		.arg(m_fields[to_num]->label());
}


bool myBoard::doMove(const QString& move, bool white_player)
{
	int from_pos, to_pos;
	if(convert_move(move, &from_pos, &to_pos)) {
		doMove(from_pos, to_pos, white_player);
		return true;
	}
	return false;
}


void myBoard::doFreeMove(int from, int to)
{
	int old_to = m_game->item(to);
	int old_from = m_game->item(from);
	m_game->setItem(to, old_from);
	m_game->setItem(from, old_to);
	fieldsSetup();
}

int myBoard::getTargetFieldSize(QSize size) {
  int w_max = size.width();
  int h_max = size.height();
  int max_size = w_max < h_max ? w_max : h_max;
  return (max_size - 2*frameWidth()) / 8;
}

void myBoard::resizeEvent(QResizeEvent* e) {
  if (m_theme) {
    int size = getTargetFieldSize(e->size());
    m_theme->setTargetSize(size);
    fieldsSetup();
  }
}

void myBoard::mousePressEvent(QMouseEvent* me)
{
    if(me->button() != Qt::LeftButton)
      return;
    if (animation_in_progress)
      return;
    for (int i = 0; i < 32; i++) {
      if (m_fields[i]->rect().contains(me->pos())) {
        invalidate();
        update();
        emit fieldClicked(i);
        return;
      }
    }
}

void myBoard::highlight(int from, int to, bool bottom_player) {
  animation_in_progress = true;
  m_src_field = m_fields[from];
  m_dst_field = m_fields[to];
  animating_bottom_player = bottom_player;
}

void myBoard::timerEvent(QTimerEvent* e) {
  if (e->timerId() != animation_timer) {
    return;
  }
  if (animation_in_progress) {
    qreal progress = ((qreal)current_step) / ((qreal)animation_steps);
    QColor begin;
    QColor end;
    if (animating_bottom_player) {
      begin = QColor(0, 0, 0);
      end = QColor(0, 255, 0);
    } else {
      begin = QColor(0, 0, 0);
      end = QColor(255, 0, 0);
    }
    m_src_color = myColorInterpolator(begin, end, progress);
    m_dst_color = myColorInterpolator(begin, end, 1-progress);
    current_step ++;
    if (current_step >= animation_steps) {
      current_step = 0;
      animation_in_progress = false;
      m_src_field = NULL;
      m_dst_field = NULL;
      invalidate();
      repaint();
    } else {
      invalidate();
      repaint();
    }
  }
}

