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
#include<SDL.h>
#include<SDL_mixer.h>
#include"menu.h"
#include"engine.h"
#include"cfg.h"
#include"control.h"
#include"SDL_plus.h"

extern SDL_Surface *screen, *gfx;
extern SDL_Joystick *joy[2];
extern Mix_Chunk *efx[8];
extern Mix_Music *bgm;
extern int sound;

extern pDesc player[2];

extern score hiscore[10];
extern cfg conf;

extern float scroll,scroll2;

void soundLoad(void);

void
drawGetName(char *name, int place, int playern)
{
	char buffer[64];

	/* erase the screen */
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));

	writeCString(gfx, screen, 90, 40, "congratulations", 0);

	sprintf(buffer,"player %i with score %.6li",playern,player[playern-1].score);
	writeCString(gfx, screen, 10, 80, buffer, 1);

	switch(place) {
		default:
			sprintf(buffer,"you got %ith place",place);
		break;
		case 1:
			sprintf(buffer,"you got %ist place",place);
		break;
		case 2:
			sprintf(buffer,"you got %ind place",place);
		break;
		case 3:
			sprintf(buffer,"you got %ird place",place);
		break;
	}
	writeCString(gfx, screen, 10, 97, buffer, 1);

	writeCString(gfx, screen, 10, 131, "enter your name", 0);

	if(name[0])
		sprintf(buffer,"%s+",name);
	else
		strcpy(buffer,"+");

	writeCString(gfx, screen, 175, 131, buffer, 1);

	SDL_Flip(screen);
}

int
getName(char *name, int place, int playern)
{
	Uint32 tick;
	SDL_Event mevent;
	int pos=0, i=0;
	char ckey='a';

	if(joy[playern-1] && player[playern-1].joy)
	{
		name[pos]=ckey;
		name[pos+1]=0;
	}

	drawGetName(name,place,playern);

	tick=SDL_GetTicks();
	while(1) {
		while(SDL_WaitEvent(&mevent)) {//PollEvent(&mevent)
    			if (mevent.type==SDL_QUIT)
    				return 0;

			/* joystick control */
			/*if(joy[playern-1] && player[playern-1].joy)
			{
				SDL_JoystickUpdate();

				i=SDL_JoystickGetAxis(joy[playern-1],1);
				if(i>4200)
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_DOWN;
				}
				if(i<-4200)
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_UP;
				}
				i=SDL_JoystickGetAxis(joy[playern-1],0);
				if(i>4200)
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_RIGHT;
				}

				if(SDL_JoystickGetButton(joy[playern-1], 0))
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_RETURN;
				}

				if(SDL_JoystickGetButton(joy[playern-1], 1))
				{
					pos++;
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_BACKSPACE;
				}
			}*/

		
			if(mevent.type==SDL_KEYDOWN) {
				if(mevent.key.keysym.sym==SDLK_ESCAPE) {
					if(!name[0])
						strcpy(name,"nobody");
					return 1;
				}

				if(mevent.key.keysym.sym==SDLK_DOWN)
				{
					ckey--;

					if(ckey<'0')
						ckey='z';
					if(ckey+1=='a')
						ckey='9';

					name[pos]=ckey;
					name[pos+1]=0;
					drawGetName(name,place,playern);

					continue;
				}

				if(mevent.key.keysym.sym==SDLK_UP)
				{
					ckey++;

					if(ckey>'z')
						ckey='0';
					if(ckey-1=='9')
						ckey='a';

					name[pos]=ckey;
					name[pos+1]=0;
					drawGetName(name,place,playern);

					continue;
				}
			
				if(mevent.key.keysym.sym==SDLK_RIGHT)
				{
					if(pos<15) {//8
						name[pos]=ckey;
						pos++;
						name[pos]=0;
						drawGetName(name,place,playern);

						ckey='a';
						continue;
					}
				}
				
				if(mevent.key.keysym.sym==SDLK_RETURN)
					if(name[0]) {
						/* pirutupiiii */
						if(sound && efx[7])
							Mix_PlayChannel(-1,efx[7],0);
						return 1;
					}

				if(mevent.key.keysym.sym==SDLK_BACKSPACE) {
						if(pos>0) {
							pos--;
							name[pos]=0;
							drawGetName(name,place,playern);
						}
						continue;
				}

				/* I don't know if this will work ever, in my system does */
				if((mevent.key.keysym.sym>=SDLK_a &&
					mevent.key.keysym.sym<=SDLK_z) ||
					(mevent.key.keysym.sym>=SDLK_0 &&
					mevent.key.keysym.sym<=SDLK_9)) {
						if(pos<15) {//8
							name[pos]=SDLK2ascii(mevent.key.keysym.sym);
							pos++;
							name[pos]=0;
							drawGetName(name,place,playern);

							ckey='a';
						}
				}
			}
		}
	}

	return 0;
}

void
drawHiscores(int max)
{
	int i;
	//SDL_Rect a,b;

	/* erase the screen */
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));

	/* DD2 characters
	a.x=60;
	a.y=5;
	b.x=450;
	b.y=43;
	b.w=211;
	b.h=190;
	SDL_BlitSurface(gfx, &b, screen, &a);*/

	/* header */
	writeCString(gfx, screen, 80, 2, "the hall of fame", 1);

	writeCString(gfx, screen, 65, 125, "requires full version", 1);

	SDL_Flip(screen);
}

int
hiscores()
{
	Uint32 tick;
	SDL_Event mevent;
	int i;
	
	drawHiscores(1);

	tick=SDL_GetTicks();
	while(1) {
		while(SDL_WaitEvent(&mevent)) {//PollEvent(&mevent)
    		if (mevent.type==SDL_QUIT)
    			return 0;
			if(mevent.type==SDL_KEYDOWN) {
				if(mevent.key.keysym.sym==SDLK_ESCAPE)
					return 1;
			}
		}
		/* wait some time and return */
		if(SDL_GetTicks()-tick>10000) {
			/* pirutupiiii */
			if(sound && efx[7])
				Mix_PlayChannel(-1,efx[7],0);
			return 1;
		}
	}

	return 0;
}

void
drawConfigure(int option)
{
	/* erase the screen */
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
	
	/*header*/
	writeCString(gfx, screen, 105, 2, "options", 1);	

	/* options */
	
	
	writeCString(gfx, screen, 20, 23,  "sound quality", 0);//gfx, screen, 20, 70,  "sound", 0
	switch(conf.sound) {
		default:
		case SOUND_HI:
			writeCString(gfx, screen, 20, 40,  "   high", (option==2));//gfx, screen, 20, 87,  "   high", (option==2)//option==3
		break;
		case SOUND_MED:
			writeCString(gfx, screen, 20, 40,  "   medium", (option==2));//3
		break;
		case SOUND_LOW:
			writeCString(gfx, screen, 20, 40,  "   low", (option==2));//3
		break;
		case NO_SOUND:
			writeCString(gfx, screen, 20, 40,  "   no sound", (option==2));//3
		break;
	}
	writeCString(gfx, screen, 160, 23,  "music", 0);
	if(conf.music)
		writeCString(gfx, screen, 160, 40,  "   on", (option==3));
	else
		writeCString(gfx, screen, 160, 40,  "   off", (option==3));

	writeCString(gfx, screen, 20, 70,  "controls", 0);//gfx, screen, 20, 23,  "player 1", 0
	if(conf.control[0]==KEYBOARD){
		writeCString(gfx, screen, 20, 87,  "   keyboard", (option==1));//gfx, screen, 20, 40,  "   keyboard", (option==1)
		writeCString(gfx, screen, 110, 115, "up f",1);
		writeCString(gfx, screen, 25, 140, "left e  down d  right x",1);	
	} 
	else {
		writeCString(gfx, screen, 20, 87,  "   accelerometer", (option==1));
		writeCString(gfx, screen, 70, 125, "requires full version",1);		
		}
	writeCString(gfx, screen, 80, 165, "tap screen to fire",1);

	/*writeCString(gfx, screen, 20, 139,  "graphic mode", 0);
	if(conf.fullscreen)
		writeCString(gfx, screen, 20, 156,  "   fullscreen", (option==4));
	else
		writeCString(gfx, screen, 20, 156,  "   windowed", (option==4));*/	

	SDL_Flip(screen);
}

int
configure()
{
	SDL_Event mevent;
	int option=1,i;
	drawConfigure(option);

	while(1) {
		while(SDL_WaitEvent(&mevent)) {//PollEvent(&mevent)
    			if (mevent.type==SDL_QUIT)
    				return 0;

			/* joystick control for the menu */
			if(joy[0])
			{
				SDL_JoystickUpdate();

				i=SDL_JoystickGetAxis(joy[0],0);//(joy[0],1)
				if(i>-12600)//4200
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_UP;
					SDL_Delay(300);	
				}
				if(i<-21000)//-4200
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_DOWN;
					SDL_Delay(300);	
				}

				if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))//SDL_JoystickGetButton(joy[0], 0)
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_RETURN;
					SDL_Delay(400);
				}

				/*if(SDL_JoystickGetButton(joy[0], 1))//
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_ESCAPE;
				}*/
			}

			if(mevent.type==SDL_KEYDOWN) {
				if(mevent.key.keysym.sym==SDLK_ESCAPE)
					return 1;
				if(mevent.key.keysym.sym==SDLK_DOWN ||
					mevent.key.keysym.sym==SDLK_d) {
					option++;
					if(option>3)//4
						option=1;
					drawConfigure(option);
				}
				if(mevent.key.keysym.sym==SDLK_UP ||
					mevent.key.keysym.sym==SDLK_f) {
					option--;
					if(option<1)
						option=3;//4
					drawConfigure(option);
				}
				if(mevent.key.keysym.sym==SDLK_RETURN) {
					switch(option) {
						default:
						break;
						case 1:
								conf.control[0]=conf.control[0] ? 0 : 1;
								drawConfigure(option);
							
						break;
						case 2://3
							conf.sound--;
							if(conf.sound<0)
								conf.sound=3;

							if(sound) {
								/*if(bgm) {
									Mix_FreeMusic(bgm);
									bgm=NULL;
								}*/

								for(i=0;i<NUM_EFX;i++)
									if(efx[i]) {
										Mix_FreeChunk(efx[i]);
										efx[i]=NULL;
									}
								//Mix_CloseAudio();
							}

							if(conf.sound!=NO_SOUND) {
								switch(conf.sound) {
										default:
										case SOUND_HI:
											i=44100;
										break;
										case SOUND_MED:
											i=22050;
										break;
										case SOUND_LOW:
											i=16000;
										break;
								}
								if(Mix_OpenAudio(i, MIX_DEFAULT_FORMAT, 2, 2048)<0) {
									fprintf(stderr, "Unable to set audio: %s\n", SDL_GetError());
									sound=0;
								} else {
									soundLoad();
									if(efx[7])
										Mix_PlayChannel(-1,efx[7],0);
									sound=1;
								}
							}
							drawConfigure(option);
						break;
						case 3:
							conf.music=conf.music ? 0 : 1;
							drawConfigure(option);
								if(conf.music==0){
									if(bgm){
										Mix_FreeMusic(bgm);
										bgm=NULL;
									}
								} else 
									soundLoad();		
						break;
						/*case 4:
							conf.fullscreen=conf.fullscreen ? 0 : 1;
							drawConfigure(option);
						break;*/
					}
				}
			}
		}
	}

	return 0;
}

void
drawMenu(int option)
{
	SDL_Rect a,b;

	/* erase the screen */
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));

	/* BETA */
	a.x=97;//77
	a.y=20;
	b.x=100;
	b.y=46;
	b.w=125;//166
	b.h=15;
	SDL_BlitSurface(gfx, &b, screen, &a);

	/* options */
	writeCString(gfx, screen, 105, 50,  "play", (option==1));//orig diff=17 now 26
	//writeCString(gfx, screen, 105, 67,  "two players", (option==2));
	writeCString(gfx, screen, 105, 76, "hall of fame", (option==2));//105, 94  option==3
	writeCString(gfx, screen, 105, 102, "options", (option==3));//105, 111 option==4
	writeCString(gfx, screen, 105, 128, "about", (option==4));//105, 138 option==5
	writeCString(gfx, screen, 105, 154, "exit game", (option==5));//105, 155 option==6

	/* some credit
	a.x=154;
	a.y=184;
	b.x=268;
	b.y=57;
	b.w=166;
	b.h=16;
	SDL_BlitSurface(gfx, &b, screen, &a);*/

	SDL_Flip(screen);
}

int
menu()
{
	SDL_Event mevent;
	int option=1, i;

	/* pirutupiiii */
	if(efx[7])
		Mix_PlayChannel(-1,efx[7],0);

	drawMenu(option);

	/* some dirty init */
	scroll=scroll2=0;

	while(1) {
		while(SDL_WaitEvent(&mevent)) {
			if (mevent.type==SDL_QUIT)
				return 0;

			/* joystick control for the menu */
			if(joy[0])
			{
				SDL_JoystickUpdate();

				i=SDL_JoystickGetAxis(joy[0],0);//(joy[0],1)
				if(i>-12600)//4200
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_UP;
					SDL_Delay(300);	
				}
				if(i<-21000)//-4200
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_DOWN;
					SDL_Delay(300);
				}

				if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_RETURN;
				}

				/*if(SDL_JoystickGetButton(joy[0], 1))
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_ESCAPE;
				}*/
			}
			
			if(mevent.type==SDL_KEYDOWN) {
				if(mevent.key.keysym.sym==SDLK_ESCAPE)
					return 0;
				if(mevent.key.keysym.sym==SDLK_DOWN ||
					mevent.key.keysym.sym==SDLK_d) {//SDLK_s
					option++;
					if(option>5)//6
						option=1;
					drawMenu(option);
				}
				if(mevent.key.keysym.sym==SDLK_UP ||
					mevent.key.keysym.sym==SDLK_f) {//SDLK_w
					option--;
					if(option<1)
						option=5;
					drawMenu(option);
				}
				if(mevent.key.keysym.sym==SDLK_RETURN) {
					switch(option) {
						default:
						break;
						case 1:
							player[0].shield=10;
							player[1].shield=0;
							player[0].score=player[1].score=0;
							player[0].stage=player[1].stage=0;
							return 1;
						/*case 2:
							player[0].shield=10;
							player[1].shield=10;
							player[0].score=player[1].score=0;
							player[0].stage=player[1].stage=0;
							return 1;*/
						case 2://3
							if(!hiscores())
								return 0;
							drawMenu(option);
						break;
						case 3://4
							if(!configure())
								return 0;
							drawMenu(option);
						break;
						case 4://5
							if(!credits())
								return 0;
							drawMenu(option);
						break;
						case 5://6
							return 0;
						break;
					}
				}
			}
		}
	}

	return 0;
}

void
drawCredits()
{
	SDL_Rect a,b;
	
	/* erase the screen */
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));

	/* BETA */ 
	a.x=97;//77
	a.y=20;
	b.x=100;
	b.y=46;
	b.w=125;//166
	b.h=15;
	SDL_BlitSurface(gfx, &b, screen, &a);

	writeCString(gfx, screen, 20, 50, "this is dd2 free version", 0);
	writeCString(gfx, screen, 20, 80, "ported by", 1);
	writeCString(gfx, screen, 40, 105, "trilli technology", 0);
	writeCString(gfx, screen, 55, 145, "try the full version", 1);
	
	SDL_Flip(screen);
}

int
credits()
{
	Uint32 tick;
	SDL_Event mevent;

	drawCredits();

	tick=SDL_GetTicks();
	while(1) {
		while(SDL_WaitEvent(&mevent)) {//PollEvent(&mevent)
    		if (mevent.type==SDL_QUIT)
    			return 0;
			if(mevent.type==SDL_KEYDOWN) {
				if(mevent.key.keysym.sym==SDLK_ESCAPE)
					return 1;
			}
		}
		/* wait some time and return */
		if(SDL_GetTicks()-tick>10000) {
			/* pirutupiiii */
			if(sound && efx[7])
				Mix_PlayChannel(-1,efx[7],0);
			return 1;
		}
	}

	return 0;
}

