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

#ifndef FIELD_H
#define FIELD_H

#include <QtWidgets>
#include <qpixmap.h>

#include "theme.h"


class Field : public QObject
{
    Q_OBJECT

public:
    Field(QObject*, int num);
    ~Field();

    //QSize sizeHint() const;
    //void resizeEvent(QResizeEvent*);

    const QString& label() const { return m_label; }
    void setLabel(const QString&);
    void showLabel(bool s, bool above);

    void showFrame(bool);

    void setTheme(Theme*);
    void setPattern(int);
    void set(int, bool);

    int number() const { return m_number; }

    int width() const {
      const QPixmap& checker = m_theme->getMan1(true);
      return checker.width();
    }

    int height() const {
      const QPixmap& checker = m_theme->getMan1(true);
      return checker.height();
    }

    void fontUpdate() { draw(); }

    void beginSetup();
    void endSetup();
    void draw(QPainter&, QRect);
    void invalidate();

    const QRect& rect() const;

signals:
    void click(int);

protected:

    //void paintEvent(QPaintEvent*);
    //void mousePressEvent(QMouseEvent*);

private:
    void draw();
    //void setPicture(QPixmap*);

    int m_number;

    // pixmap = pattern + label + picture + frame;

    /*QPixmap* m_frame;
    QPixmap* m_checker;
    QPixmap* m_pattern;*/
    int m_pattern_id;
    int m_checker_id;
    bool m_bottom_is_white;

    QString m_label;
    bool m_show_label;
    bool m_show_above;

    QPixmap* pixmap;
    bool pixmap_valid;

    bool show_frame;

    bool setup_mode;
    Theme* m_theme;
    QRect m_rect;
};

#endif


