///////////////////////////////////////////////////////////////////////////////
//Copywrite (c) 2021-2022. Eleanor Morris. STU# 2996720. All Rights Reserved.
//This program is free software; you can redistribute it and/or
//modify it.
//
//This software was written for COMP 369
//Assignment 2
//Practical Game Programming
//Athabasca University
///////////////////////////////////////////////////////////////////////////////

#include <allegro.h>
#include <stdlib.h>
#include <stdio.h>

#define SPIRAL_BMP 0
#define WHITE makecol(255,255,255)
#define NUM 15
#define NUMPOIS 15
#define NUMBULLETS 5

typedef struct SPRITE
{
	int x, y;
	int speed;
	int height, width;
	int direction;
	int count;
	int active;
} SPRITE;

//variables
BITMAP *welcome_bmp;
BITMAP *buffer;
BITMAP *sidebuffer; 
BITMAP *mainbuffer; 
BITMAP *spiral_bmp;
SPRITE spiral;
SPRITE *spir = &spiral;
BITMAP *enemy_bmp;
SPRITE enemy[NUM];
SPRITE *en[NUM];
BITMAP *bullet_bmp;
SPRITE bullets;
SPRITE *bull = &bullets;
BITMAP *poison_bmp;
SPRITE poison[NUMPOIS];
SPRITE *pois[NUMPOIS];
BITMAP *over_bmp;
SPRITE over;
SPRITE *gover = &over;
SAMPLE *shoot;
SAMPLE *music;
SAMPLE *gfinish;
SAMPLE *bfinish;
BITMAP *explode_bmp;
BITMAP * didit_bmp;
SPRITE didit;
SPRITE *did = &didit;
DATAFILE *data;
int volume = 128;
int pan = 128;
int pitch = 1000;
int play;
int encount, health, score, bullcount, ending;

//declare methods
void initialize();
void movespiral(int direction);
void setupspiral();
void getkeypressed();
void setupenemy(int n);
void warpenemies(int n);
void moveenemies(int n);
void drawsprite(SPRITE *spr);
void erasesprite(BITMAP *bmp, SPRITE *spr);
void fire();
void updatebullet();
int inside(int x,int y,int left,int top,int right,int bottom);
void endgame(int ending);
void playmusic();
void setupstats();
void menu();
void displayhelpmenu();
void restartgame();
void setuppoison(int n);
void updatepoison();
void explode();



//initialize stuff
void initialize()
{
    allegro_init();
	install_keyboard();
	set_color_depth(24);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
    srand(time(NULL));
    install_sound(DIGI_AUTODETECT, MIDI_NONE, ""); 
	data = load_datafile("GetThem.dat");    
}

//set up enemies
void setupenemy(int n)
{
	int dir = rand() % 2;
	en[n] = &enemy[n];
	en[n]->height = enemy_bmp->h;
	en[n]->width = enemy_bmp->w;
	en[n]->x = 50 + (rand() % (int)(SCREEN_W - 50 + 1));	
	en[n]->y = rand() % 200;
	en[n]->speed = 1;
	en[n]->active = 1;
	
	if (dir == 0) en[n]->direction = 1;
	if (dir == 1) en[n]->direction = -1;
	en[n]->count;
}

void setuppoison(int n)
{
	//set up enemy poisin
	pois[n] = &poison[n];
	pois[n]->x = en[n]->x;
	pois[n]->y = en[n]->y + 3;
	pois[n]->width = poison_bmp->w;
	pois[n]->height = poison_bmp->h;
	pois[n]->speed = 3;
	pois[n]->active = 1;
}

void updatepoison()
{
	int n;
	for(n=0; n<NUMPOIS; n++)
	{
		pois[n]->y +=pois[n]->speed;		
		
		if (inside(pois[n]->x, pois[n]->y, spir->x, spir->y, 
				(spir->x+spir->width), (spir->y + spir->height)))
				{
						health --;	
					if (health < 1) {
						health = 0;
						ending = 0;
						explode();
						endgame(ending);
					}
				}		
	}
	   
}

//setup the spiral
void setupspiral()
{
    //load the spiral bitmap and screen
	spiral_bmp = (BITMAP *) data[SPIRAL_BMP].dat;
	spir->width = 50;
	spir->height = 50;
	//set up start position
    spir->x = SCREEN_W/2 - 50/2;
    spir->y = mainbuffer->h - 50;
	spir->speed = 4;
	
	//set up spirals bullets
	bullet_bmp = load_bitmap("bullet.bmp", NULL);	
	bull->x = spir->x + 25;
	bull->y = spir->y -10;
    bull->width = bullet_bmp->w;
    bull->height = bullet_bmp->h;	
	
	health = 50;	
	score = 0;	
	ending = 1;
}

void setupover()
{
	over_bmp = load_bitmap("gameover.bmp", NULL);
	gover->width = over_bmp->w;
	gover->height = over_bmp->h;
	gover->x = mainbuffer->w/2 - gover->width/2;
	gover->y = mainbuffer->h/2 - gover->height/2 +100;
}
 void setupdidit()
 {
 	didit_bmp = load_bitmap("didit.bmp", NULL);
 	did->width = didit_bmp->w;
 	did->height = didit_bmp->h;
 	did->x = (buffer->w/2 - did->width/2) - 25;
 	did->y = (buffer->h/2 - did->height/2) - 200;
 }

//update bullet poition
void updatebullet()
{
	if (!bull->active) return;
	bull->y -= bull->speed;	
	if (bull->y < 0) bull->active = 0;
	
	int n;
	for (n=0; n<NUM; n++)
	{
		if(inside(bull->x, bull->y, en[n]->x, en[n]->y, 
				(en[n]->x+en[n]->width), (en[n]->y + en[n]->height)))
		{
			bull->active = 0;
			en[n]->active = 0;
			en[n]->x = -20;      
			en[n]->y = -20;      
			encount --;
			score ++;
			if (encount < 1) 
			{
				ending = 1;
				endgame(ending);	
			}
		}
	}
}

//collision inside method
int inside(int x,int y,int left,int top,int right,int bottom)
{
    if (x > left && x < right && y > top && y < bottom)
        return 1;
    else
        return 0;
}

//fire at enemies
void fire()
{
	
	if(!bull->active) 
	{	
		bull->active = 1;
		bull->speed = 15;
		bull->x = spir->x + 25;
		bull->y = spir->y - 10;	
		play_sample(shoot, volume, pan, pitch, FALSE);
		bullcount++;
		if (bullcount >3) 
		{
			int n;
			for(n=0; n<NUMPOIS; n++) setuppoison(n);
			bullcount = 0;
		}
		
	 } 	
}
//move enemies
void moveenemies(int n)
{	
if(!en[n]->active) return;


	int delay = rand() % 10000;
	if (en[n]->x < 0) en[n]->direction *= -1; 
	if (en[n]->x > SCREEN_W - en[n]->width) en[n]->direction *= -1;
	
	en[n]->count ++;
	if (en[n]->count > delay) 
	{
		en[n]->direction *= -1;
		en[n]->count = 0;
	 } 
	
	if (en[n]->direction == -1) en[n]->x -= en[n]->speed;
	if (en[n]->direction == 1) en[n]->x += en[n]->speed;

}

//move the spiral
void movespiral(int direction)
{
	 
    switch(direction)
   	{
    	case 0:
    		spir->x -= spir->speed;
    		break;
    	case 1:
    		spir->x += spir->speed;
    		break;
	}
	if(spir->x < -20) spir->x = -20;
	if(spir->x > SCREEN_W-30) spir->x = SCREEN_W-30;
	//add dont let go off acreen		
}



//function for erasing sprites. All sprites are on the mainbuffer
void erasesprite(BITMAP *bmp, SPRITE *spr)
{
    //erase the sprite
    rectfill(bmp, spr->x, spr->y, spr->x+spr->width, spr->y+spr->height, 0);
}

//get keyboard input
void getkeypressed()
{
    if (key[KEY_LEFT]) movespiral(0);
	if (key[KEY_RIGHT]) movespiral(1);
	if (key[KEY_SPACE]) fire();
	if (key[KEY_Y]) restartgame();
	if (key[KEY_LCONTROL] && key[KEY_H]) displayhelpmenu();
	if (key[KEY_LCONTROL] && key[KEY_M]){
		play *= -1;
		if (play == 1){
			play_sample(music, volume, pan, pitch, TRUE);
		}else{
			stop_sample(music);	
			}
	}				
}

//play background song
void playmusic()
{ 
	shoot = load_sample("shoot.wav");
	music = load_sample("backmusic.wav");
	gfinish = load_sample("goodfinish.wav");
	bfinish = load_sample("game-over.wav"); 
	play_sample(music, volume, pan, pitch, TRUE);
	play = 1;			      	
}

//end the game//display end of game and stop music
void endgame(int ending)
{
	//over_bmp = load_bitmap("gameover.bmp", NULL);
	stop_sample(music);
		
	if (ending == 1 ) 
	{
		play_sample(gfinish, volume, pan, pitch, FALSE);
		draw_sprite(mainbuffer, didit_bmp, did->x, did->y);	
	}
	if (ending == 0)  play_sample(bfinish, volume, pan, pitch, FALSE);
	draw_sprite(mainbuffer, over_bmp, mainbuffer->w/2 - gover->width/2, (mainbuffer->h/2 - gover->height/2)+100);
	int n;
	for (n=0; n<NUM; n++) erasesprite(mainbuffer, en[n]);
	textprintf_ex(mainbuffer,font, 235, 350, WHITE, 0, "Press 'y' to play again or Esc to quit");	
}

void explode()
{
    int n;

    //load explode image
    if (explode_bmp == NULL)
    {
        explode_bmp = load_bitmap("explode.bmp", NULL);
    }
    
    //draw the explosion bitmap several times
    for (n = 0; n < 5; n++)
    {
        rotate_sprite(screen, explode_bmp, 
            spir->x + rand()%50-10, spir->y + rand()%50-10, 
            itofix(rand()%255));

        rest(10);
    }  
}

//setup side screen
void setupstats()
{
	rectfill(sidebuffer, 5, 5, SCREEN_W-5, SCREEN_H-10, WHITE);
	rect(sidebuffer, 6, 6, SCREEN_W - 6, SCREEN_H - 11 , 0);
	textprintf_ex(sidebuffer,font,10,10,0, -1, "---Status Menu---");
	textprintf_ex(sidebuffer,font,10,30,0, -1, "Enemies remaining: %i", encount);
	textprintf_ex(sidebuffer,font,10,40,0, -1, "Score: %i", score);
	textprintf_ex(sidebuffer,font,10,50,0, -1, "Health: %i", health);
	textprintf_ex(sidebuffer,font,10,70,0, -1, "Use arrow keys to move.  Use space bar to shoot.  Press ESC to quit.");	
	textprintf_ex(sidebuffer,font,10,80,0, -1, "Hold CTRL-H for the help menu.");	
}

//display menue screen
void menu()
{
	rectfill(buffer, 5, 5, buffer->w, buffer->h, 0);
	textout_centre_ex(buffer, font, "COMP 369 - Practical Game Programming", buffer->w /2, 10, WHITE, -1);
	textout_centre_ex(buffer, font, "Welcome to my first game.  This game was creating using Allegro 4.2.2.", buffer->w /2, 30, WHITE, -1);
	blit(welcome_bmp, buffer, 0, 0, buffer->w/2 - welcome_bmp->w/2, buffer->h/2 - welcome_bmp->h/2 - 100, welcome_bmp->w, welcome_bmp->h);
	textout_centre_ex(buffer, font,"Press 'p' to play. ESC to exit.", buffer->w /2, 400, WHITE, -1);
	stretch_sprite(buffer, spiral_bmp, buffer->w/4, buffer->h-100, 100, 100);
	draw_sprite(buffer, enemy_bmp, buffer->w-100, buffer->h-200);
	draw_sprite(buffer, enemy_bmp, buffer->w-80, buffer->h-250);
	draw_sprite(buffer, enemy_bmp, 100, buffer->h-150);
	draw_sprite(buffer, enemy_bmp, 200, buffer->h-200);
	draw_sprite(buffer, enemy_bmp, buffer->w-300, buffer->h-300);



	blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);    
}

//display help menue
void displayhelpmenu()
{
	rectfill(sidebuffer, 5, 5, SCREEN_W-5, SCREEN_H-10, WHITE);
	rect(sidebuffer, 6, 6, SCREEN_W - 6, SCREEN_H - 11 , 0);
	textprintf_ex(sidebuffer,font,20, 10, 0, -1, "---'Get Them!' Help Menu---");	
	textprintf_ex(sidebuffer,font,20, 30, 0, -1, "Object: Destroy all the enemies");	
	textprintf_ex(sidebuffer,font,20, 40, 0, -1, "ESC to quit");	
	textprintf_ex(sidebuffer,font,20, 50, 0, -1, "Left Arrow Key  <- : move left");	
	textprintf_ex(sidebuffer,font,20, 60, 0, -1, "Right Arrow Key  -> : move right");	
	textprintf_ex(sidebuffer,font,20, 70, 0, -1, "Space Bar : shoot");	
	textprintf_ex(sidebuffer,font,20, 80, 0, -1, "CTRL-M : turn music off/on");
	textprintf_ex(sidebuffer,font,20, 90, 0, -1, "CTRL-H: help menu");	
}

//reload the game
void restartgame()
{
    //initialize enemies
    erasesprite(mainbuffer, gover);
    erasesprite(mainbuffer, did);
    enemy_bmp = load_bitmap("enemy.bmp", NULL);
    encount = NUM;
    int n;
	for (n=0; n<NUM; n++) setupenemy(n);
	health = 50;
	score = 0;
	ending = 1;
	//restart music
	playmusic();
}
	
////////////////////////////////////////////////////////////////////
//main function
////////////////////////////////////////////////////////////////////
int main()
{
	int n;	
    //initialize the program
	initialize();
	playmusic();

	buffer = create_bitmap(SCREEN_W, SCREEN_H);
	sidebuffer = create_bitmap(SCREEN_W, 100);
	mainbuffer = create_bitmap(SCREEN_W, 500);
	welcome_bmp = load_bitmap("welcome.bmp", NULL);
	
	//initialize spiral
    setupspiral();
    setupover();
    setupdidit();
    
    //initialize enemies
    poison_bmp = load_bitmap("poison.bmp", NULL);
    enemy_bmp = load_bitmap("enemy.bmp", NULL);
   // enemy_bmp = load_bitmap("enemy.bmp", NULL);
    encount = NUM;
	for (n=0; n<NUM; n++) setupenemy(n);
	bullcount = 0;
	for (n=0; n<NUMPOIS; n++) setuppoison(n);
	
	while (!key[KEY_P])
	{
	   menu();
	   if(key[KEY_ESC]) allegro_exit();
	}

	//main loop
    while (!key[KEY_ESC])
	{		

	
		//clear the screen and update the sprites
        erasesprite(mainbuffer, spir);
        for (n=0; n<NUM; n++) erasesprite(mainbuffer, en[n]);
        erasesprite(mainbuffer, bull);
        
        for (n=0; n<NUMPOIS; n++) erasesprite(mainbuffer, pois[n]);
        
        setupstats();
        if (keypressed) getkeypressed();
		for(n=0; n<NUM; n++) moveenemies(n);
		updatebullet();
		updatepoison();
				
		//draw the spiral to mainbuffer
		stretch_sprite(mainbuffer, spiral_bmp, spir->x, spir->y, 50, 50);
		
		//draw bullets to mainbuffer if still active
		if (bull->active)  draw_sprite(mainbuffer, bullet_bmp, bull->x, bull->y);	
		
		//draw enemies to mainbuffer if they are still alive	
		for (n=0; n<NUM; n++) 
		{
			if(en[n]->active)	draw_sprite(mainbuffer, enemy_bmp, en[n]->x, en[n]->y);
		}
		
		
		for (n=0; n<NUMPOIS; n++)
		{
			if(pois[n]->active)  draw_sprite(mainbuffer, poison_bmp, pois[n]->x, pois[n]->y);
		}
		 		
		blit(mainbuffer, buffer,0,0,0,0,mainbuffer->w, mainbuffer->h);
		blit(sidebuffer, buffer, 0,0,0,mainbuffer->h, sidebuffer->w, sidebuffer->h);
		
       	//update the screen
		acquire_screen();
        blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);     
        release_screen();
		rest(10);
	}

    //delete and remove the stuff
    destroy_bitmap(spiral_bmp);
    destroy_bitmap(enemy_bmp);
    destroy_bitmap(bullet_bmp);
    destroy_bitmap(buffer);
    destroy_bitmap(mainbuffer);
    destroy_bitmap(sidebuffer);
    destroy_sample(shoot);
    destroy_sample(gfinish);
    destroy_sample(music);
    destroy_sample(bfinish);
    remove_sound();
    allegro_exit();
    return 0;
}
END_OF_MAIN()
