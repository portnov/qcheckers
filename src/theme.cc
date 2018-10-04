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
#include "common.h"

Theme::Theme(QObject* parent, const QString& path) : QObject(parent) {
  m_path = path;
  if (path == DEFAULT_THEME) {
		// just in case no themes installed.
    m_pat1_path = ":/icons/theme/tile1.png";
    m_pat2_path = ":/icons/theme/tile2.png";
    m_frame_path = ":/icons/theme/frame.png";

    m_man_black_path = ":/icons/theme/manblack.png";
    m_man_white_path = ":/icons/theme/manwhite.png";
    m_king_black_path = ":/icons/theme/kingblack.png";
    m_king_white_path = ":/icons/theme/kingwhite.png";
  } else {
		m_pat1_path = path+"/"THEME_TILE1;
		m_pat2_path = path+"/"THEME_TILE2;
    m_frame_path = path+"/"THEME_FRAME;

    m_man_black_path = path+"/"THEME_MANBLACK;
    m_man_white_path = path+"/"THEME_MANWHITE;
    m_king_black_path = path+"/"THEME_KINGBLACK;
    m_king_white_path = path+"/"THEME_KINGWHITE;
  }
}

QPixmap* Theme::getFrame() {
  if (! m_frame) {
    m_frame = new QPixmap(m_frame_path);
  }
  return m_frame;
}

QPixmap* Theme::getPattern1() {
  if (! m_pattern1) {
    m_pattern1 = new QPixmap(m_pat1_path);
  }
  return m_pattern1;
}

QPixmap* Theme::getPattern2() {
  if (! m_pattern2) {
    m_pattern2 = new QPixmap(m_pat2_path);
  }
  return m_pattern2;
}

QPixmap* Theme::getMan1(bool white) {
  if (white) {
    if (! m_man_white) {
      m_man_white = new QPixmap(m_man_white_path);
    }
    return m_man_white;
  } else {
    if (! m_man_black) {
      m_man_black = new QPixmap(m_man_black_path);
    }
    return m_man_black;
  }
}

QPixmap* Theme::getMan2(bool white) {
  if (! white) {
    if (! m_man_white) {
      m_man_white = new QPixmap(m_man_white_path);
    }
    return m_man_white;
  } else {
    if (! m_man_black) {
      m_man_black = new QPixmap(m_man_black_path);
    }
    return m_man_black;
  }
}

QPixmap* Theme::getKing1(bool white) {
  if (white) {
    if (! m_king_white) {
      m_king_white = new QPixmap(m_king_white_path);
    }
    return m_king_white;
  } else {
    if (! m_king_black) {
      m_king_black = new QPixmap(m_king_black_path);
    }
    return m_king_black;
  }
}

QPixmap* Theme::getKing2(bool white) {
  if (! white) {
    if (! m_king_white) {
      m_king_white = new QPixmap(m_king_white_path);
    }
    return m_king_white;
  } else {
    if (! m_king_black) {
      m_king_black = new QPixmap(m_king_black_path);
    }
    return m_king_black;
  }
}

int Theme::getFieldWidth() {
  if (! m_man_white) {
    m_man_white = new QPixmap(m_man_white_path);
  }
  Q_ASSERT(m_man_white);
  return m_man_white->width();
}

int Theme::getFieldHeight() {
  if (! m_man_white) {
    m_man_white = new QPixmap(m_man_white_path);
  }
  Q_ASSERT(m_man_white);
  return m_man_white->height();
}

