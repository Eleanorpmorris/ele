///////////////////////////////////////////////////////////////////////////////
//Emec's Adventure
//
//Copywrite (c) 2021-2022. Eleanor Morris. STU# 2996720. All Rights Reserved.
//This program is free software; you can redistribute it and/or
//modify it.
//
//This software was written for COMP 369
//Assignment 3
//Practical Game Programming
//Athabasca University
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <allegro.h>
#include "mappyal.h"

#define MODE GFX_AUTODETECT_WINDOWED
#define WHITE makecol(255,255,255)
#define RED makecol(220,20,60)
#define WIDTH 640
#define SWIDTH 175
#define HEIGHT 480
#define JUMPIT 1600
#define NUMCOINS 3

#define BACKGROUND_BMP 0
#define BACKMUSIC_WAV 1
#define BAD_BMP 2
#define COIN_BMP 3
#define GOOD_ENDING_BMP 4
#define PICKUP_COIN18_WAV 5
#define PR2_BMP 6
#define PR3_BMP 7
#define PR4_BMP 8
#define PR5_BMP 9
#define HLP_BMP 10
#define TITLE_BMP 11
#define WALK_BMP 12

//define the sprite structure
typedef struct SPRITE
{
    int dir, alive;
    int x,y;
    int width,height;
    int xspeed,yspeed;
    int xdelay,ydelay;
    int xcount,ycount;
    int curframe,maxframe,animdir;
    int framecount,framedelay;
    int happy, sad;
    int maybehap, maybesad;
    
}SPRITE;

//declare the bitmaps and sprites
BITMAP *player_image[6];
BITMAP *princess_bitmap;
BITMAP *princess_bitmap2;
BITMAP *princess_bitmap3;
BITMAP *princess_bitmap4;
BITMAP *princess_bitmap5;
BITMAP *coin_bmp;
BITMAP *bad_ending_bitmap;
BITMAP *good_ending_bitmap;
BITMAP *title_bmp;
//SPRITE *title;
SPRITE *coin[NUMCOINS];
SPRITE thecoin[NUMCOINS];
SPRITE *princess;
SPRITE *player;
SPRITE *bad_end;
SPRITE *good_end;
BITMAP *background;
BITMAP *buffer;	
BITMAP *sideBuffer;
BITMAP *temp;
SAMPLE *collect;
SAMPLE *background_music;
SAMPLE *ending;
int volume = 250;
int pan = 128;
int pitch = 1000;
int play;
int coinkeeper=0;
int distanceLeft=0;
int counter;
int gameover=0;
int flag = 0;
double fuzzyanswer;

DATAFILE *data;

//tile grabber
BITMAP *grabframe(BITMAP *source, 
                  int width, int height, 
                  int startx, int starty, 
                  int columns, int frame)
{
    BITMAP *temp = create_bitmap(width,height);
    int x = startx + (frame % columns) * width;
    int y = starty + (frame / columns) * height;
    blit(source,temp,x,y,0,0,width,height);
    return temp;
}


int collided(int x, int y)
{
    BLKSTR *blockdata;
	blockdata = MapGetBlock(x/mapblockwidth, y/mapblockheight);
	return blockdata->tl;
}

//collision inside method
int inside(int x,int y,int left,int top,int right,int bottom)
{
    if (x > left && x < right && y > top && y < bottom)
        return 1;
    else
        return 0;
}

void setUpEndings()
{
	bad_end = malloc(sizeof(SPRITE));
	good_end = malloc(sizeof(SPRITE));
	bad_end->x = WIDTH/3 - bad_ending_bitmap->w/2;
	bad_end->y = HEIGHT/3 - bad_ending_bitmap->h/2;
	good_end->x = WIDTH/3 - good_ending_bitmap->w/2;
	good_end->y = HEIGHT/3 - good_ending_bitmap->h/2;
}

void setupCoins(int n)
{
	coin[n] = malloc(sizeof(SPRITE));
	coin[n] = &thecoin[n];
	coin[n]->height = coin_bmp->h;
	coin[n]->width = coin_bmp->w;
	coin[n]->x = 50 + (rand() % (int)(SCREEN_W - 50 + 1));	
	coin[n]->y = 0;
	coin[n]->yspeed = (rand() % 2) +1;
	coin[n]->alive = 1;
}

void deactivateCoins()
{
	int m;
	for (m=0; m<NUMCOINS; m++) coin[m]->alive = 0;
}

updateCoins(int n)
{
	coin[n]->y += coin[n]->yspeed;
	if (inside((coin[n]->x + 20), (coin[n]->y + 20), player->x, player->y, (player->x+player->width + 20), (player->y+player->height + 20)))
	{
		coinkeeper ++;
		play_sample(collect, volume, pan, pitch, FALSE);
		rest(rand() % 50 + 1);
		coin[n]->y = 0;
		coin[n]->x = player->x + ((rand() % 500) + 1);
	}		
	
	
	if (coin[n]->y >= 420)
	{
		rest(rand() % 50 + 1);
		coin[n]->y = 0;
		coin[n]->x = player->x + ((rand() % 500) + 1);
	}
}

//set up player
void setUpPlayer()
{
	if(gameover == 0)
	{
		play_sample(background_music, volume, pan, pitch, FALSE); 
	}
    player = malloc(sizeof(SPRITE));
    player->x = 100;
    player->y = 10;
    player->curframe=0;
    player->framecount=0;
    player->framedelay=6;
    player->maxframe=5;
    player->width=player_image[0]->w;
    player->height=player_image[0]->h;
}

void setUpPrincess()
{
	princess_bitmap = (BITMAP *) data[HLP_BMP].dat;
	princess_bitmap2 = (BITMAP *) data[PR2_BMP].dat;
	princess_bitmap3 = (BITMAP *) data[PR3_BMP].dat;
	princess_bitmap4 = (BITMAP *) data[PR4_BMP].dat;
	princess_bitmap5 = (BITMAP *) data[PR5_BMP].dat;
	princess = malloc(sizeof(SPRITE));
	princess->x = 4700;
	princess->y = 146;
	princess->xspeed = 1; 
	princess->width = princess_bitmap->w;
	princess->height = princess_bitmap->h;
	princess->alive = 0;
	princess->happy = 0;
	princess->sad = 0;
	princess->maybehap = 0;
	princess->maybesad = 0;
}

void playending()
{
	flag = 1;
	play_sample(ending, volume, pan, pitch, FALSE);
}

void getFuzzyLogic()
{
	int answer = coinkeeper;
	if (answer >= 37)
	{
		fuzzyanswer = 1.0;
	} 
	else if (answer <= 29)
	{
		fuzzyanswer = 0.0;
	}
	else
	{
		fuzzyanswer =  (answer-29.9)/(37.0-30.0);
	}
	
	
}


void updatePrincess()
{
	if(player->x >= 4350 )
	{
		princess->x -= princess->xspeed;
		if(princess->x <= 4576)
		{
			princess->x = 4576;
			princess->alive = 1;
			
			//start fuzzy locig here
			getFuzzyLogic();
	
			if (fuzzyanswer == 1.0)
			{
				princess->happy = 1;
			}
			else if (fuzzyanswer == 0.0)
			{
				princess->sad = 1 ;
			}
			else if(fuzzyanswer > 0.5)
			{
				princess -> maybehap = 1;
			}
			else if (fuzzyanswer <= 0.5)
			{
				princess -> maybesad = 1;
			}
		}
	}
	draw_sprite(princess_bitmap, buffer, princess->x, princess->y);
}

void die()
{
	stop_sample(background_music);
   	rectfill(screen, 0, 0, WIDTH, HEIGHT, RED);
   	rest(50);
   	rectfill(screen, 0, 0, WIDTH, HEIGHT, 4);
   	rest(50);
   	rectfill(screen, 0, 0, WIDTH, HEIGHT, WHITE);
   	rest(50);
   	rectfill(screen, 0, 0, WIDTH, HEIGHT, 0);
   	rest(50);
   	rectfill(screen, 0, 0, WIDTH, HEIGHT, RED);
   	rest(50);
   	rectfill(screen, 0, 0, WIDTH, HEIGHT, 6);
	rest(50);
	rectfill(screen, 0, 0, WIDTH, HEIGHT, WHITE);
	rest(50);
	coinkeeper = 0;
	counter = 0;
		
}
void checkPlayerPosition(int x, int y)
{
    if(y > 400)
    {
    	die();
       	setUpPlayer();
       	
	}
	
	if(x < -30)
	{
		player->x = -30;
	}
	
	if(y < -50)
	{
		player->y = -50;
	}	
	
	if(x > 4280)
	{
		deactivateCoins();
		counter = 0;
		distanceLeft= 0;
	}
}

int checkTilesCollision(int facing, int oldx)
{
	int direction = facing;
	int oldpx = oldx;
	
	if (!facing) 
        { 
            if (collided(player->x, player->y + player->height)) 
                player->x = oldpx; 
        }
		else 
        { 
            if (collided(player->x + player->width, 
                player->y + player->height)) 
                player->x = oldpx; 
        }
    return oldpx;    	
}


int checkJump(int jumping)
{
	int jump = jumping;
	
   if (jump==JUMPIT)
   { 
	    if (!collided(player->x + player->width/2, player->y + player->height + 5))
            jump = 0; 

	    if (key[KEY_SPACE]) 
       		jump = 30;
    }
    else
    {
        player->y -= jump/3; 
        jump--; 
    }

	if (jump<0) 
    { 
        if (collided(player->x + player->width/2, player->y + player->height))
		{ 
            jump = JUMPIT; 
            while (collided(player->x + player->width/2, player->y + player->height))
           		 player->y -= 2; 
        } 
    }
	return jump;
}


//check key presses
int checkKeyPresses(int facing)
{
    int direction = facing;
   	if (key[KEY_RIGHT]) 
    { 
        direction = 1; 
        player->x+=4; 
        if (++player->framecount > player->framedelay)
        {
            player->framecount=0;
            if (++player->curframe > player->maxframe)
                player->curframe=1;
        }
    }
    else if (key[KEY_LEFT]) 
    { 
        direction = 0; 
        player->x-=4; 
        if (++player->framecount > player->framedelay)
        {
            player->framecount=0;
            if (++player->curframe > player->maxframe)
                player->curframe=1;
        }
    }
    else player->curframe=0;
    
    distanceLeft = princess->x - player->x;
	
    return direction;
}

int updateMapx(int x)
{
	int mapxoff = x;
	        //update the map scroll position
		mapxoff = player->x + player->width/2 - WIDTH/2 + 10;


        //avoid moving beyond the map edge
		if (mapxoff < 0) mapxoff = 0;
		if (mapxoff > (mapwidth * mapblockwidth - WIDTH))
            mapxoff = mapwidth * mapblockwidth - WIDTH;
        
    return mapxoff;
}

int updateMapy(int y)
{
	int mapyoff = y;
	mapyoff = player->y + player->height/2 - HEIGHT/2 + 10;

	if (mapyoff < 0) 
            mapyoff = 0;
	if (mapyoff > (mapheight * mapblockheight - HEIGHT)) 
            mapyoff = mapheight * mapblockheight - HEIGHT;
            
    return mapyoff;
}
		
//display help menu
void displayhelpmenu()
{
	rect(buffer, 4, 4, WIDTH/2+1, HEIGHT/4+1, 0);
	rectfill(buffer, 5, 5, WIDTH/2, HEIGHT/4, WHITE);
	rect(buffer, 6, 6, WIDTH/2-2, HEIGHT/4-2 , 0);
	textprintf_ex(buffer,font,20, 10, 0, -1, "---'Emec's Adventure'' Help Menu---");	
	textprintf_ex(buffer,font,20, 30, 0, -1, "Collect magic stars for the princess!");	
	textprintf_ex(buffer,font,20, 40, 0, -1, "");	
	textprintf_ex(buffer,font,20, 50, 0, -1, "Left Arrow Key  <- : Move left");	
	textprintf_ex(buffer,font,20, 60, 0, -1, "Right Arrow Key  -> : Move right");	
	textprintf_ex(buffer,font,20, 70, 0, -1, "Space Bar : Jump");	
	textprintf_ex(buffer,font,20, 80, 0, -1, "ESC to quit");
	textprintf_ex(buffer,font,20, 90, 0, -1, "CTRL-H: help menu");	
	acquire_screen();	
	blit(buffer, screen, 0, 0, 0, 0, WIDTH-1, HEIGHT-1);
	release_screen();
}
//setup side screen
void sideScreen()
{
	rectfill(sideBuffer, 0, 0, SWIDTH, HEIGHT/4, WHITE);
	rect(sideBuffer, 0, 0, SWIDTH, HEIGHT/4 -1, 0);
	rect(sideBuffer, 2, 2, SWIDTH-2, HEIGHT/4 -2, 0);
	textprintf_ex(sideBuffer,font,8,8,0, -1, "--Status Menu--");
	textprintf_ex(sideBuffer,font,10,40,0, -1, "Coins: %i", coinkeeper);
	textprintf_ex(sideBuffer,font,10,50,0, -1, "Distance to go: %i", distanceLeft);	
	textprintf_ex(sideBuffer,font,10,60,0, -1, "Time remaining: %i", counter);
	textprintf_ex(sideBuffer,font,10,70,0, -1, "Answer: %1.2f", fuzzyanswer);	
	textprintf_ex(sideBuffer,font,10,90,0, -1, "Help menu: CTRL-H");
	textprintf_ex(sideBuffer,font,10,100,0, -1, "Press ESC to quit");	
}


//start screen
void menu()
{
	blit(background, buffer, 0, 0, 0, 0, WIDTH, HEIGHT);
	textout_centre_ex(buffer, font, "COMP 369 - Practical Game Programming", buffer->w /2, 10, 0, -1);
	draw_sprite(buffer, title_bmp, buffer->w/2-title_bmp->w/2, buffer->h-420);
	double x = 2.0/7.0;
	textout_centre_ex(buffer, font, "Copywrite (c) 2021-2022. Created By: Eleanor Morris.", buffer->w /2, 30, 0, -1);
	textout_centre_ex(buffer, font, "Your mission is to deliver coins to the princess.", buffer->w /2, 120, 0, -1);
	textout_centre_ex(buffer, font, "The princess must have 30 coins to pay her rent before the goblin eats her.", buffer->w /2, 200, 0, -1);
	textout_centre_ex(buffer, font, "You can help by collecting star coins.", buffer->w /2, 220, 0, -1);
	textout_centre_ex(buffer, font, "Get 30 coins and deliver them to her before its too late!", buffer->w /2, 240,0, -1);
	textout_centre_ex(buffer, font,"Press 'p' to play. ESC to exit.", buffer->w /2, 400, 0, -1);
	draw_sprite(buffer, princess_bitmap, buffer->w-100, buffer->h-200);
	blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);   
}

timer1(void)
{
	counter++;
}

void checkTime()
{
	if(counter > 600)
   	{
   		setUpPlayer();
    	die();
    	int n;
    	for (n=0; n<NUMCOINS; n++) coin[n]->alive=1;
    	counter = 0;
	}
}


void restart()
{
	setUpPlayer();
	setUpPrincess();
	setUpEndings;
	int m;
	for (m=0; m<NUMCOINS; m++) setupCoins(m);
//	gameover = 0;
	counter = 0;
	coinkeeper = 0;
	distanceLeft = 0;
	flag = 0;	
}
void initialize()
{
    int n;
	
	allegro_init();	
	install_timer();
	install_keyboard();
	set_color_depth(16);
	srand(time(NULL));
	set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
	install_sound(DIGI_AUTODETECT, MIDI_NONE, "");  
	install_timer();
}

void garbageCollection()
{
	int n;
	 for (n=0; n<6; n++) destroy_bitmap(player_image[n]);
    free(princess);    
    free(player);
    free(bad_end);
    free(good_end);
    for (n=0; n<NUMCOINS; n++) free(coin[n]);
    //destroy_sample(shoot);
	destroy_bitmap(buffer);
	destroy_bitmap(sideBuffer);
	MapFreeMem ();
	
}


int main (void)
{
    int mapxoff, mapyoff;
    int oldpy, oldpx;
    int facing = 0;
    int jump = JUMPIT;
    int n;

	initialize();
	data = load_datafile("emec.dat");
	title_bmp = (BITMAP *) data[TITLE_BMP].dat;
	collect = (SAMPLE *) data[PICKUP_COIN18_WAV].dat;
	background_music = (SAMPLE *) data[BACKMUSIC_WAV].dat;
	//ending = load_sample("ending_song");
	
	MapLoad("myplatformer.fmp");
	background = (BITMAP *) data[BACKGROUND_BMP].dat;
	bad_ending_bitmap = (BITMAP *) data[BAD_BMP].dat;
	good_ending_bitmap = (BITMAP *) data[GOOD_ENDING_BMP].dat;
	
	temp = (BITMAP *) data[WALK_BMP].dat;
    for (n=0; n<6; n++)
    	player_image[n] = grabframe(temp,50,70,0,0,6,n);
    destroy_bitmap(temp);

    setUpPlayer();
    setUpPrincess();
    setUpEndings();
      
    coin_bmp = (BITMAP *) data[COIN_BMP].dat;
	int m;
	for (m=0; m<NUMCOINS; m++) setupCoins(m);	

    //create the double buffer
	buffer = create_bitmap (WIDTH, HEIGHT);
	clear(buffer);
	sideBuffer = create_bitmap(SWIDTH, HEIGHT/4);
	clear(sideBuffer);
	
	LOCK_VARIABLE(counter);
	LOCK_FUNCTION(timer1);
	install_int(timer1, 1000);


	while (!key[KEY_P])
	{
	   menu();
	   if(key[KEY_ESC]) allegro_exit();
	}
	
    //main loop
	while (!key[KEY_ESC])
	{	
		if (key[KEY_LCONTROL] && key[KEY_H]) displayhelpmenu();
		
		oldpy = player->y; 
	    oldpx = player->x;
        
        
    if(gameover != 1)
	{
	    checkPlayerPosition(oldpx, oldpy);
        
	    if(keypressed) 
	    {
    	 	facing = checkKeyPresses(facing);
     		jump = checkJump(jump);
     		oldpx =  checkTilesCollision(facing, oldpx);
     		mapxoff = updateMapx(mapxoff);
     		mapyoff = updateMapy(mapyoff);
     		int num;
     		for(num=0; num < NUMCOINS; num++){
     			updateCoins(num);
			 }
			updatePrincess();
     		
    	}
    	


        //draw the background tiles
		MapDrawBG(buffer, mapxoff, mapyoff, 0, 0, WIDTH-1, HEIGHT-1);

        //draw foreground tiles
		MapDrawFG(buffer, mapxoff, mapyoff, 0, 0, WIDTH-1, HEIGHT-1, 0);
			//draw the princess
		if(princess->alive == 0)
		{		
			draw_sprite(buffer, princess_bitmap, (princess->x-mapxoff), (princess->y));
		}
		if(princess->alive == 1 && princess->happy == 1)
		{
			draw_sprite(buffer, princess_bitmap2, (princess->x-mapxoff), (princess->y));
			draw_sprite(buffer, good_ending_bitmap, good_end->x, good_end->y);
			gameover=1;
		}
		else if (princess->alive == 1 && princess->sad == 1)
		{
			draw_sprite(buffer, princess_bitmap3, (princess->x-mapxoff), (princess->y));
			draw_sprite(buffer, bad_ending_bitmap, bad_end->x, bad_end->y);
			gameover=1;
		}
		else if (princess->alive == 1 && princess->maybesad == 1)
		{
			draw_sprite(buffer, princess_bitmap5, (princess->x-mapxoff), (princess->y));
			draw_sprite(buffer, good_ending_bitmap, good_end->x, good_end->y);
			gameover=1;
		}
		else if (princess->alive == 1 && princess->maybehap == 1)
		{
			draw_sprite(buffer, princess_bitmap4, (princess->x-mapxoff), (princess->y));
			draw_sprite(buffer, good_ending_bitmap, good_end->x, good_end->y);
			gameover=1;
		}
		
		if(gameover==1)
		{
			restart();
		}
		//draw coins to mainbuffer if they are still active	
    	int c;
		for (c=0; c<NUMCOINS; c++) 
		{
			if(coin[c]->alive)	draw_sprite(buffer, coin_bmp, (coin[c]->x-mapxoff), (coin[c]->y-mapyoff));
		}

        //draw the player's sprite
		if (facing) 
            draw_sprite(buffer, player_image[player->curframe], 
                (player->x-mapxoff), (player->y-mapyoff+1));
		else 
            draw_sprite_h_flip(buffer, player_image[player->curframe], 
                (player->x-mapxoff), (player->y-mapyoff));
                

		//draw the stats
    	sideScreen();
		checkTime();
 } else {
 	if(key[KEY_ESC]) allegro_exit();
	if(key[KEY_Y])
	{
		gameover = 0;
		stop_sample(background_music);
		restart();
	 } 
 }
        //blit the double buffer 
		vsync();
        acquire_screen();
        blit(sideBuffer, buffer, 0, 0, WIDTH-200, 5, SWIDTH, HEIGHT/4);
		blit(buffer, screen, 0, 0, 0, 0, WIDTH-1, HEIGHT-1);
        release_screen();

	} //while

	garbageCollection();
	allegro_exit();
	return 0;
}
END_OF_MAIN()

