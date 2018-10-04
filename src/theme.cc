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

#include <QSettings>
#include <QPainter>
#include <QSvgRenderer>
#include <QMouseEvent>
#include <QDebug>
#include <QFileInfo>

#include "theme.h"
#include "common.h"

Theme::Theme(QObject* parent, const QString& path, int size) : QObject(parent) {
  m_path = path;
  settings = new QSettings(path + "/"THEME_FILE, QSettings::IniFormat, this);
  qInfo("Theme: %s\n", qUtf8Printable(name()));

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
		m_pat1_path = path+"/" + settings->value("tile1", THEME_TILE1).toString();
		m_pat2_path = path+"/" + settings->value("tile2", THEME_TILE2).toString();
    m_frame_path = path+"/" + settings->value("frame", THEME_FRAME).toString();

    m_man_black_path = path+"/" + settings->value("man_black", THEME_MANBLACK).toString();
    m_man_white_path = path+"/" + settings->value("man_white", THEME_MANWHITE).toString();
    m_king_black_path = path+"/" + settings->value("king_black", THEME_KINGBLACK).toString();
    m_king_white_path = path+"/" + settings->value("king_white", THEME_KINGWHITE).toString();
  }
  m_target_size = size;
}

bool Theme::isValid() {
  if (m_path == DEFAULT_THEME) {
    return true;
    } else {
      if (! QFileInfo::exists(m_pat1_path) ||
          ! QFileInfo::exists(m_pat2_path) ||
          ! QFileInfo::exists(m_frame_path) ||
          ! QFileInfo::exists(m_man_black_path) ||
          ! QFileInfo::exists(m_man_white_path) ||
          ! QFileInfo::exists(m_king_white_path) ||
          ! QFileInfo::exists(m_king_black_path)) {
        qWarning("one of required files does not exist. ");
        return false;
      }
      return true;
    }
}

const QString Theme::name() {
  QFileInfo info(m_path);
  return settings->value("name", info.baseName()).toString();
}

QPixmap* Theme::getFrame() {
  if (! m_frame) {
    m_frame = render(m_frame_path);
  }
  return m_frame;
}

QPixmap* Theme::getPattern1() {
  if (! m_pattern1) {
    m_pattern1 = render(m_pat1_path);
  }
  return m_pattern1;
}

QPixmap* Theme::getPattern2() {
  if (! m_pattern2) {
    m_pattern2 = render(m_pat2_path);
  }
  return m_pattern2;
}

QPixmap* Theme::getMan1(bool white) {
  if (white) {
    if (! m_man_white) {
      m_man_white = render(m_man_white_path);
    }
    return m_man_white;
  } else {
    if (! m_man_black) {
      m_man_black = render(m_man_black_path);
    }
    return m_man_black;
  }
}

QPixmap* Theme::getMan2(bool white) {
  if (! white) {
    if (! m_man_white) {
      m_man_white = render(m_man_white_path);
    }
    return m_man_white;
  } else {
    if (! m_man_black) {
      m_man_black = render(m_man_black_path);
    }
    return m_man_black;
  }
}

QPixmap* Theme::getKing1(bool white) {
  if (white) {
    if (! m_king_white) {
      m_king_white = render(m_king_white_path);
    }
    return m_king_white;
  } else {
    if (! m_king_black) {
      m_king_black = render(m_king_black_path);
    }
    return m_king_black;
  }
}

QPixmap* Theme::getKing2(bool white) {
  if (! white) {
    if (! m_king_white) {
      m_king_white = render(m_king_white_path);
    }
    return m_king_white;
  } else {
    if (! m_king_black) {
      m_king_black = render(m_king_black_path);
    }
    return m_king_black;
  }
}

int Theme::getFieldWidth() {
  if (! m_man_white) {
    m_man_white = render(m_man_white_path);
  }
  Q_ASSERT(m_man_white);
  return m_man_white->width();
}

int Theme::getFieldHeight() {
  if (! m_man_white) {
    m_man_white = render(m_man_white_path);
  }
  Q_ASSERT(m_man_white);
  return m_man_white->height();
}

void Theme::setTargetSize(int size) {
  if (m_target_size != size) {
    m_target_size = size;

    // reset cached pixmaps,
    // so that they will be re-rendered
    m_man_white = NULL;
    m_man_black = NULL;
    m_king_white = NULL;
    m_king_black = NULL;

    m_pattern1 = NULL;
    m_pattern2 = NULL;
    m_frame = NULL;
  }
}

bool Theme::getIsResizeable() {
  // FIXME
  return m_man_white_path.endsWith(".svg");
}

QPixmap* Theme::render(const QString& path) {
  int size = m_target_size;
  if (path.endsWith(".svg")) {
    QSvgRenderer renderer(path);
    if (size == 0) {
      size = renderer.defaultSize().width();
    }
    //qInfo("SVG: %s, size: %d", qUtf8Printable(path), size);
    QPixmap result(size,size);
    result.fill(Qt::transparent);
    QPainter paint(&result);
    renderer.render(&paint);
    return new QPixmap(result);

  } else {
    if (m_target_size == 0) {
      return new QPixmap(path);
    } else {
      QPixmap* result = new QPixmap(path);
      return new QPixmap(result->scaled(size, size));
    }
  }
}

