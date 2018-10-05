/***************************************************************************
 *   Copyright (C) 2002-2003 Andi Peredri                                  *
 *   andi@ukr.net                                                          *
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

#ifndef THEME_H
#define THEME_H

#include <QtWidgets>
#include <qpixmap.h>

class Theme : public QObject {
  Q_OBJECT

  public:
    Theme(QObject*, const QString&, int);

    QPixmap& getMan1(bool);
    QPixmap& getMan2(bool);
    QPixmap& getKing1(bool);
    QPixmap& getKing2(bool);

    QPixmap& getChecker(int, bool);

    QPixmap& getPattern1();
    QPixmap& getPattern2();
    QPixmap& getPattern(int);
    QPixmap& getFrame();

    void setTargetSize(int);
    int getFieldWidth();
    int getFieldHeight();

    bool getIsResizeable();

    bool isValid();
    void reset();

    const QString name();

  private:

    void render(const QString&, QPixmap& result, bool&);
    QSettings* settings;

    QString m_path;
    int m_target_size;

    QString m_man_white_path;
    QString m_man_black_path;
    QString m_king_white_path;
    QString m_king_black_path;

    QString m_pat1_path;
    QString m_pat2_path;
    QString m_frame_path;

    QPixmap m_man_white;
    bool m_man_white_valid;
    QPixmap m_man_black;
    bool m_man_black_valid;
    QPixmap m_king_white;
    bool m_king_white_valid;
    QPixmap m_king_black;
    bool m_king_black_valid;

    QPixmap m_pattern1;
    bool m_pattern1_valid;
    QPixmap m_pattern2;
    bool m_pattern2_valid;
    QPixmap m_frame;
    bool m_frame_valid;

    QPixmap m_empty_pixmap;

};

#endif
