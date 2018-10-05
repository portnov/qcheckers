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
#include "checkers.h"

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
  //m_target_size = size == 0 ? MAX_TILE_SIZE : size;

  reset();
}

void Theme::reset() {
  m_man_white_valid = false;
  m_man_black_valid = false;
  m_king_black_valid = false;
  m_king_white_valid = false;
  m_pattern1_valid = false;
  m_pattern2_valid = false;
  m_frame_valid = false;
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

QPixmap& Theme::getFrame() {
  render(m_frame_path, m_frame, m_frame_valid);
  return m_frame;
}

QPixmap& Theme::getPattern1() {
  render(m_pat1_path, m_pattern1, m_pattern1_valid);
  return m_pattern1;
}

QPixmap& Theme::getPattern2() {
  render(m_pat2_path, m_pattern2, m_pattern2_valid);
  return m_pattern2;
}

QPixmap& Theme::getPattern(int i) {
  if (i == 1) {
    return getPattern1();
  } else {
    return getPattern2();
  }
}

QPixmap& Theme::getMan1(bool white) {
  if (white) {
    render(m_man_white_path, m_man_white, m_man_white_valid);
    return m_man_white;
  } else {
    render(m_man_black_path, m_man_black, m_man_black_valid);
    return m_man_black;
  }
}

QPixmap& Theme::getMan2(bool white) {
  if (! white) {
    render(m_man_white_path, m_man_white, m_man_white_valid);
    return m_man_white;
  } else {
    render(m_man_black_path, m_man_black, m_man_black_valid);
    return m_man_black;
  }
}

QPixmap& Theme::getKing1(bool white) {
  if (white) {
    render(m_king_white_path, m_king_white, m_king_white_valid);
    return m_king_white;
  } else {
    render(m_king_black_path, m_king_black, m_king_black_valid);
    return m_king_black;
  }
}

QPixmap& Theme::getKing2(bool white) {
  if (! white) {
    render(m_king_white_path, m_king_white, m_king_white_valid);
    return m_king_white;
  } else {
    render(m_king_black_path, m_king_black, m_king_black_valid);
    return m_king_black;
  }
}

QPixmap& Theme::getChecker(int item, bool white) {
  switch (item) {
    case MAN1:
      return getMan1(white);
    case MAN2:
      return getMan2(white);
    case KING1:
      return getKing1(white);
    case KING2:
      return getKing2(white);
    default:
      return m_empty_pixmap;
  }
}

int Theme::getFieldWidth() {
  render(m_man_white_path, m_man_white, m_man_white_valid);
  int result = m_man_white.width();
  return result;
  //return result == 0 ? MAX_TILE_SIZE : result;
}

int Theme::getFieldHeight() {
  render(m_man_white_path, m_man_white, m_man_white_valid);
  int result = m_man_white.height();
  return result;
  //return result == 0 ? MAX_TILE_SIZE : result;
}

void Theme::setTargetSize(int size) {
  /*if (size == 0) {
    size = MAX_TILE_SIZE;
    qInfo("Size is 0, set it to %d", size);
  }*/
  if (m_target_size != size) {
    qInfo("Size: %d -> %d", m_target_size, size);
    m_target_size = size;

    // reset cached pixmaps,
    // so that they will be re-rendered
    reset();
  }
}

bool Theme::getIsResizeable() {
  // FIXME
  return m_man_white_path.endsWith(".svg");
}

void Theme::render(const QString& path, QPixmap& result, bool& valid) {
  if (valid) {
    //qDebug("Already valid: %s", qUtf8Printable(path));
    return;
  }
  int size = m_target_size;
  valid = true;
  if (path.endsWith(".svg")) {
    QSvgRenderer renderer(path);
    if (size == 0) {
      size = renderer.defaultSize().width();
    }
    qInfo("Rendering %s: %d", qUtf8Printable(path), size);
    Q_ASSERT(size > 0);
    result = QPixmap(size, size);
    result.fill(Qt::transparent);
    QPainter paint(&result);
    renderer.render(&paint);

  } else {
    qInfo("Rendering %s: %d", qUtf8Printable(path), size);
    if (size == 0) {
      result = QPixmap(path);
    } else {
      Q_ASSERT(size > 0);
      result = QPixmap(path).scaled(size, size);
    }
  }
}

