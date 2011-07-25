/*

    Dodgin' Diamond 2, a shot'em up arcade
    Copyright (C) 2003,2004 Juan J. Martinez <jjm@usebox.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/
#ifndef _MENU_H_
#define _MENU_H_

void drawGetName(char *name, int place, int playern);
int getName(char *name, int place, int playern);

void drawHiscores(int max);
int hiscores();

void drawConfigure(int option);
int configure();

void drawMenu(int option);
int menu();

void drawCredits();
int credits();

int clickDetect(int menu,int button, int x, int y);

#define NUM_EFX		8

// enter button size and position
#define PLAYBOXSTARTX 100
#define PLAYBOXLENGTH 75
#define PLAYBOXSTARTY 45
#define PLAYBOXHEIGHT 26

// clear button size and position
#define HOFBOXSTARTX 100
#define HOFBOXLENGTH 130
#define HOFBOXSTARTY 71
#define HOFBOXHEIGHT 26

// shuffle button size and position
#define OPTIONBOXSTARTX 100
#define OPTIONBOXLENGTH 80
#define OPTIONBOXSTARTY 97
#define OPTIONBOXHEIGHT 26

// solve button size and position
#define ABOUTBOXSTARTX 100
#define ABOUTBOXLENGTH 75
#define ABOUTBOXSTARTY 123
#define ABOUTBOXHEIGHT 26

// quit button size and position
#define QUITBOXSTARTX 100
#define QUITBOXLENGTH 105
#define QUITBOXSTARTY 149
#define QUITBOXHEIGHT 30

// enter button size and position
#define QUALITYBOXSTARTX 45
#define QUALITYBOXLENGTH 105
#define QUALITYBOXSTARTY 40
#define QUALITYBOXHEIGHT 25

// enter button size and position
#define MUSICBOXSTARTX 195
#define MUSICBOXLENGTH 60
#define MUSICBOXSTARTY 40
#define MUSICBOXHEIGHT 25

// enter button size and position
#define CONTROLBOXSTARTX 45
#define CONTROLBOXLENGTH 145
#define CONTROLBOXSTARTY 85
#define CONTROLBOXHEIGHT 25

#endif
