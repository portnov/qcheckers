TEMPLATE= app
TARGET	= qcheckers
CONFIG  += qt warn_on debug

isEmpty(PREFIX) {
  PREFIX = /usr/local
}

DEFINES += PREFIX=\\\"$$PREFIX\\\"

QT += widgets svg

HEADERS	= pdn.h \
	    checkers.h echeckers.h rcheckers.h \
	    capture.h theme.h field.h toplevel.h view.h history.h board.h \
	    newgamedlg.h \
	    common.h \
	    player.h humanplayer.h computerplayer.h
	   

SOURCES	= pdn.cc \
	    checkers.cc echeckers.cc rcheckers.cc \
	    capture.cc theme.cc field.cc toplevel.cc view.cc history.cc board.cc \
	    main.cc \
	    newgamedlg.cc \
	    humanplayer.cc computerplayer.cc

RESOURCES = ../qcheckers.qrc ../icons.qrc


# prefix can be set in common.h
#PREFIX		= $$system(sh ../helpers/extract_prefix.sh)
message(PREFIX: $$PREFIX)

TRANSLATIONS	= ../lang/qcheckers_de.ts ../lang/qcheckers_fr.ts ../lang/qcheckers_ru.ts

target.path	= $$PREFIX/bin
INSTALLS	+= target

#DESTDIR	= /usr/local
#message(Destdir: $$DESTDIR)


#
# This hack is needed for i18n support.
#
share.path	+= $$PREFIX/share/qcheckers
share.files	+= qcheckers.pdn ../COPYING ../AUTHORS ../ChangeLog ../README ../themes ../lang
INSTALLS	+= share

# Install desktop entry
desktop.path += $$PREFIX/share/applications
desktop.files += ../qcheckers.desktop
INSTALLS += desktop

# Install icon
icon.path += $$PREFIX/share/icons/hicolor/scalable/apps
icon.files += ../qcheckers.svg
INSTALLS += icon
