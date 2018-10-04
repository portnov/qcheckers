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
    Theme(QObject*, const QString&);

    QPixmap* getMan1(bool);
    QPixmap* getMan2(bool);
    QPixmap* getKing1(bool);
    QPixmap* getKing2(bool);

    QPixmap* getPattern1();
    QPixmap* getPattern2();
    QPixmap* getFrame();

    int getFieldWidth();
    int getFieldHeight();

  private:
    QString m_path;

    QString m_man_white_path;
    QString m_man_black_path;
    QString m_king_white_path;
    QString m_king_black_path;

    QString m_pat1_path;
    QString m_pat2_path;
    QString m_frame_path;

    QPixmap* m_man_white = NULL;
    QPixmap* m_man_black = NULL;
    QPixmap* m_king_white = NULL;
    QPixmap* m_king_black = NULL;

    QPixmap* m_pattern1 = NULL;
    QPixmap* m_pattern2 = NULL;
    QPixmap* m_frame = NULL;

};

#endif
