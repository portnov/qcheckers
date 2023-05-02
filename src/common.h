/***************************************************************************
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

#ifndef _COMMON_H_
#define _COMMON_H_


#define	APPNAME		"QCheckers"
#define VERSION		"0.9.0"
#define EXT		"pdn"


#define HOMEPAGE	"https:/portnov.github.io/qcheckers"
#define COPYRIGHT	"(c) 2002-2003, Andi Peredri (andi@ukr.net)<br>" \
			"(c) 2004-2007, Artur Wiebe (wibix@gmx.de)"
#define CONTRIBS	"Sebastien Prud'homme (prudhomme@laposte.net)<br>" \
			"Guillaume Bedot (guillaume.bedot@wanadoo.fr)<br>" \
			"Ilya V. Portnov (portnov84@rambler.ru)"

#ifndef PREFIX
#define PREFIX		"/usr/local"
#endif

#define USER_PATH	".qcheckers"		// in $HOME
#define THEME_DIR	"themes/"

// some keys for QSettings
#define CFG_KEY		"/"APPNAME"/"

//
#define DEFAULT_THEME	"Default"
//
#define THEME_TILE1	"tile1.png"
#define THEME_TILE2	"tile2.png"
#define THEME_FRAME	"frame.png"
#define THEME_MANBLACK	"manblack.png"
#define THEME_MANWHITE	"manwhite.png"
#define THEME_KINGBLACK	"kingblack.png"
#define THEME_KINGWHITE	"kingwhite.png"
#define THEME_FILE	"theme"

//
#define MAX_TILE_SIZE	64

// Qt5 / 6 compatibility workarounds

#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
#define QT_ENDL endl
#else
#define QT_ENDL Qt::endl
#endif

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#define QT_SKIP_EMPTY_PARTS QString::SkipEmptyParts
#else
#define QT_SKIP_EMPTY_PARTS Qt::SkipEmptyParts
#endif

#endif

