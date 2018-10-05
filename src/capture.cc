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

#include <QPair>
#include <QList>

#include "capture.h"

Captures::Captures(int from, int capt, int to) {
  m_captures.append(Capture(from, to, capt));
}

Captures::Captures(const Captures& capture) {
  m_captures.append(capture.m_captures);
}

Captures::Captures(const Captures* capture) {
  m_captures.append(capture->m_captures);
}

bool Captures::isEmpty() const {
  return m_captures.isEmpty();
}

int Captures::size() const {
  return m_captures.size();
}

void Captures::add(Captures& capture) {
  m_captures.append(capture.m_captures);
}

void Captures::add(Captures* capture) {
  m_captures.append(capture->m_captures);
}

const Capture Captures::at(int i) const {
  return m_captures.at(i);
}

Capture::Capture(int from, int to, int capt) {
  captured = capt;
  source = from;
  target = to;
}

