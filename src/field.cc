/***************************************************************************
 *   Copyright (C) 2002-2003 Andi Peredri                                  *
 *   andi@ukr.net                                                          *
 *   Copyright (C) 2004-2005 Artur Wiebe
 *   wibix@gmx.de     
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

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

#include "theme.h"
#include "field.h"
#include "common.h"
#include "checkers.h"


Field::Field(QObject* parent,int i)
    : QObject(parent)
{
    pixmap = new QPixmap(128, 128);
    pixmap_valid = false;

    m_number=i;

    show_frame = false;

    m_show_label = true;

    setup_mode = false;

    m_pattern_id = 0;
    m_checker_id = FREE;
}

Field::~Field() {
  if (pixmap) {
    delete pixmap;
  }
}

void Field::beginSetup() {
  Q_ASSERT(! setup_mode);
  //qInfo("beginSetup(%d)", m_number);
  setup_mode = true;
}

void Field::endSetup() {
  Q_ASSERT(setup_mode);
  //qInfo("endSetup(%d)", m_number);

  setup_mode = false;
}


/*
void Field::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    draw();
    Q_ASSERT(pixmap_valid);
    p.drawPixmap(0, 0, *pixmap);

    p.end();
}
*/


/*
void Field::mousePressEvent(QMouseEvent* me)
{
    if(me->button() != Qt::LeftButton)
	return;
    emit click(m_number);
}
*/

void Field::invalidate() {
  pixmap_valid = false;
}

void Field::draw()
{
    if (pixmap_valid) {
      return;
    }
    /*if (pixmap) {
      delete pixmap;
    }*/
    /*QSize mySize = size();
    if (mySize.height() == 0 || mySize.width() == 0) {
      return;
    }*/
    //qInfo("Draw: %d x %d", mySize.width(), mySize.height());
    //pixmap = new QPixmap(128, 128);

    pixmap->fill(Qt::white);
    QPainter paint;
    paint.begin(pixmap);
    //paint.setFont(font());

    if (m_pattern_id) {
      QPixmap& pattern = m_theme->getPattern(m_pattern_id);
      if (! pattern.isNull()) {
        paint.drawPixmap(0, 0, pattern);
      }
    }

    // notation
    paint.setPen(Qt::white);
    QRect not_rect = paint.boundingRect(2, 2, 0, 0, Qt::AlignLeft, m_label);
    QPixmap& checker = m_theme->getChecker(m_checker_id, m_bottom_is_white);
    if (m_show_above) {
      if (! checker.isNull())
        paint.drawPixmap(0, 0, checker);
      if (m_show_label) {
        paint.fillRect(not_rect, Qt::black);
        paint.drawText(not_rect, Qt::AlignTop|Qt::AlignLeft, m_label);
      }
    } else {
      if (m_show_label)
        paint.drawText(not_rect, Qt::AlignTop|Qt::AlignLeft, m_label);
      if (! checker.isNull())
        paint.drawPixmap(0, 0, checker);
    }

    if (show_frame) {
      QPixmap& frame = m_theme->getFrame();
      paint.drawPixmap(0, 0, frame);
    }

    paint.end();
    //update();
    pixmap_valid = true;
}

void Field::draw(QPainter& painter, QRect rect) {
  m_rect = rect;
  draw();
  Q_ASSERT(pixmap_valid);
  painter.drawPixmap(rect.x(), rect.y(), *pixmap);
}

const QRect& Field::rect() const {
  return m_rect;
}

void Field::showFrame(bool b)
{
    if(show_frame != b) {
      show_frame = b;
      invalidate();
      draw();
    }
}

void Field::setPattern(int i)
{
  if (i != m_pattern_id) {
    m_pattern_id = i;
    invalidate();
  }
}

void Field::setTheme(Theme* theme) {
  Q_ASSERT(theme);
  m_theme = theme;
  invalidate();
  draw();
}

void Field::set(int item, bool bottom_is_white) {
  if (m_checker_id != item || m_bottom_is_white != bottom_is_white) {
    m_checker_id = item;
    m_bottom_is_white = bottom_is_white;
    invalidate();
  }
}

void Field::setLabel(const QString& str)
{
    if(m_label!=str) {
      m_label=str;
      invalidate();
    }
}


void Field::showLabel(bool s, bool a)
{
    if(s!=m_show_label || a!=m_show_above) {
      m_show_above = a;
      m_show_label = s;
      invalidate();
    }
}

/*
QSize Field::sizeHint() const {
  if (! m_theme) {
    return QSize();
  } else {
    return QSize(m_theme->getFieldWidth(), m_theme->getFieldHeight());
  }
}
*/

/*
void Field::resizeEvent(QResizeEvent* e) {
  invalidate();
  if (e->size().width() == 0 || e->size().height() == 0) {
    return;
  }
  draw();
}
*/

