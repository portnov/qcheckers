QCheckers - formerly known as KCheckers
=======================================

Qt-based checkers boardgame

This is the homepage of QCheckers, the Qt version of the classic boardgame "checkers". This game is also known as "draughts". QCheckers is distributed under the GNU GPL.

QCheckers can play english draughts and russian draughts.

* [Project on the GitHub][1]
* [GitHub issue tracker][2]
* [Project on SourceForge.net][3]

Some additional information can be found in the [FAQ](FAQ.md) file.

News
----

* 2018-10-10 Ilya Portnov
  * Move the project to github.com
  * Move to Qt5 for better HiDPI support
  * Support of SVG for themes, so that the board is scalable now
  * Change UI a bit: use docker widgets, that can be rearranged or resized
  * Indicate moves by blipping fields where from and where to the move was done.
* 2007-07-23 Artur Wiebe
	* Start moving project to sourceforge.net.
* 2005-12-19 Artur Wiebe
	* QCheckers 0.8 released.

Old news can be found at http://kcheckers.org/ (no longer available).

Downloads
---------

See [Downloads][4] for older versions. QCheckers up to version 0.6 requires the threaded version of Qt 3.0 library. From 0.6 you can download a corresponding GPG signature file to verify file's integrity.

Features
--------

  * Pure Qt.
  * Built-in checkers engine.
  * English and Russian rules of play.
  * Beginner, Novice, Average, Good, Expert and Master levels of skill.
  * Several themes of the board.
  * Portable Draughts Notation database format support.
  * Saving, loading and restarting of game.
  * Auto change of the men's color.
  * Multiple Undos/Redos.
  * Optional numeration of the board.
  * The on-line description of the play's rules.
  * Threads support.
  * Internationalization support (English, German, Russian and French). 

Screenshots
-----------

Current git master with "Simple 3D" theme:

![3D-like SVG theme](https://user-images.githubusercontent.com/284644/48266670-e0373b00-e451-11e8-930c-ce57d2ef7fdb.png)

Current git master with "Simple SVG" theme:

![Simple SVG theme](https://user-images.githubusercontent.com/284644/48266672-e0cfd180-e451-11e8-80ab-692bb6a7f16d.png)

Older screenshots:

![QCheckers 0.8](https://user-images.githubusercontent.com/284644/48266511-63a45c80-e451-11e8-902e-245ab5c7e410.png)

![KCheckers in KDE](https://user-images.githubusercontent.com/284644/48266512-643cf300-e451-11e8-85e4-ed8454e0c7f4.png)

![KCheckers on Qtopia](https://user-images.githubusercontent.com/284644/48266514-643cf300-e451-11e8-87ce-1b6feb70c17f.png)
		
Future plans
------------

* Animation of movement.
* Network Support. Peter Chiocchetti (from the XCheckers' Project) and I (Artur Wiebe) thinking about programming a checkers server or some tools to "generate" a such one.

Rules of Play
-------------

*   In the beginning of game you have 12 checkers (men).
*   The men move forward only. The men can capture:
  *     by jumping forward only (english rules);
  *     by jumping forward or backward (russian rules). 
*   A man which reaches the far side of the board becomes a king.
*   The kings move forward or backward:
  *     to one square only (english rules);
  *     to any number of squares (russian rules). 
*   The kings capture by jumping forward or backward.
*   Whenever a player is able to make a capture he must do so. 

Team
----

* Andi Peredri - original author, artist, designer, russian translation.
* Artur Wiebe - current maintainer and developer, german translation. 
* Ilya Portnov - current developer

Links
-----

*   [Portable Draughts Notation database format][5].
*   [Xcheckers boardgame][6] 

[1]: https://github.com/portnov/qcheckers
[2]: https://github.com/portnov/qcheckers/issues
[3]: http://qcheckers.sourceforge.net/
[4]: https://sourceforge.net/projects/qcheckers/files/latest/download
[5]: http://www.chessandcheckers.com/pdn.htm
[6]: http://arton.cunst.net/xcheckers

