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

#include "main.h"
#include "control.h"
#include "SDL_plus.h"

extern bool pause;
//extern event;
//SDL_Event tevent;

void control_player_joy(SDL_Joystick *joy, pDesc *p)
{
	Uint8* keys;
	Uint8* mousebut;
	
	keys=SDL_GetKeyState(NULL);		
	mousebut=SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1);	
	
	int x,y,but;
	
	if(pause)
		return;

	SDL_JoystickUpdate();
	x=SDL_JoystickGetAxis(joy,0);
	y=SDL_JoystickGetAxis(joy,1);
	
	if(x<-16800) {//-4200 //21000
		p->incy=+1;
	} 
		
	if(x>-10500) {//4200
		p->incy=-1;		
	}		
	
	if(y<-4200) {	
		p->incx=-1;
	}
	
	if(y>4200) {
		p->incx=+1;
	}

	if(x>-16800 && x<-10500) //-21000
		p->incy=0;

	if(y>-4200 && y<4200)
		p->incx=0;
		
	if((mousebut||keys[p->keys[4]]) && !p->fire)//mousebut && !p->fire
		p->fire=1;
	if(!mousebut && !keys[p->keys[4]])//!mousebut
		p->fire=0;
}

void control_player(pDesc *p)
{
	Uint8* keys;
	Uint8* mousebut;

	keys=SDL_GetKeyState(NULL);	
	mousebut=SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1);	
	

	if(pause)
		return;
	
	if(keys[p->keys[0]]) {
		p->incx=-1;
	}
	if(keys[p->keys[1]]) {
		p->incx=+1;
	}		
	if(keys[p->keys[2]]) {
		p->incy=-1;
	}
	if(keys[p->keys[3]]) {
		p->incy=+1;
	}
		
	if((!keys[p->keys[2]] && !keys[p->keys[3]]) ||
		(keys[p->keys[2]] && keys[p->keys[3]])) {
			p->incy=0;
	}
	if((!keys[p->keys[0]] && !keys[p->keys[1]]) ||
		(keys[p->keys[0]] && keys[p->keys[1]])) {
			p->incx=0;
	}
	
	//if(keys[p->keys[4]]&& !p->fire)//keys[p->keys[4]] && !p->fire    mousebut && !p->fire 
	//	p->fire=1;
	if((mousebut||keys[p->keys[4]]) && !p->fire)//keys[p->keys[4]] && !p->fire    mousebut && !p->fire 
		p->fire=1;
	if(!mousebut && !keys[p->keys[4]])//!keys[p->keys[4]] !mousebut || 
		p->fire=0;
}
