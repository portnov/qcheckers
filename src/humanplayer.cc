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

#include <QDebug>

#include "humanplayer.h"
#include "rcheckers.h"
#include "echeckers.h"
#include "pdn.h"


myHumanPlayer::myHumanPlayer(const QString& name, bool white,
	bool second_player)
: myPlayer(name, white)
{
	selected = false;

	m_second = second_player;
	m_game = 0;
}


myHumanPlayer::~myHumanPlayer()
{
}


void myHumanPlayer::yourTurn(const Checkers* g)
{
	if(!m_game || m_game->type()!=g->type()) {
		delete m_game;
		if(g->type()==RUSSIAN) {
			m_game = new RCheckers();
		} else {
			m_game = new ECheckers();
		}
	}

	// synchronize
	m_game->fromString(g->toString(m_second));
}

bool myHumanPlayer::fieldClicked(int field_num, bool* select, bool bottom_is_white, QString& errmsg)
{
	if(m_second) {
		field_num = 31 - field_num;
	}

	switch(m_game->item(field_num)) {
	case MAN1:
	case KING1: {
    Captures* capture = m_game->getPossibleCapture();
    bool must_capture = !capture->isEmpty() && !m_game->canCapture1(field_num);
    QString captureStr = m_game->describeCapture(bottom_is_white, capture);
    delete capture;
		if (must_capture) {
      errmsg = tr("You must capture. Available moves are: %1").arg(captureStr);
			return false;
		}
		if(!m_game->canCapture1(field_num)
				&& !m_game->canMove1(field_num)) {
			errmsg = tr("This unit does not have valid moves.");//TODO better text.
			return false;
		}

		// Player (re)selects
		from = field_num;
		fromField = field_num;
		selected = true;
		*select = true;
		return true;
    break;
  }

	case FREE:
		if(!selected) {
			return true;
		}

	  if (!go(field_num)) {
      errmsg = tr("This unit cannot be moved to field you pointed.");
      return false;	// incorrect course
		}

		// move done - unselect
		if(selected) {
			*select = false;
		}
		selected = false;

		emit moveDone(m_game->toString(m_second));
	break;

	default:
	break;
	}

	return true;
}


bool myHumanPlayer::go(int to)
{
	return m_game->go1(from, to);
}

