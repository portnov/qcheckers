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

#ifndef CAPTURE_H
#define CAPTURE_H

#include <QPair>
#include <QList>

class Capture {
  public:
    Capture(int, int, int);
    ~Capture() { }

    int captured;
    int source;
    int target;
};

class Captures {
  public:
    Captures() { }
    Captures(int, int, int);
    Captures(const Captures&);
    Captures(const Captures*);
    ~Captures() { }

    void add(Captures&);
    void add(Captures*);

    bool isEmpty() const;
    int size() const;

    const Capture at(int) const;

  private:
    QList<Capture> m_captures;
};

#endif

