#include "Hero.h"

Hero::Hero()
{}

void Hero::Destroy()
{
	GameObject::Destroy();
}

void Hero::Init(ALLEGRO_BITMAP *image)
{
	GameObject::Init(WIDTH/2, HEIGHT/2, 3, 3, 0, 0, 14, 17);
	SetID(HERO);
	SetAlive(true);
	lives = 3;

	maxFrame = 2;
	curFrame = 1;
	frameWidth = 14;
	frameHeight = 18;
	animationColumns = 1;
	animationDirection = 1;
	animationDelay = 5;
	animationRow = 0;
	framecount = 1;

	if(image != NULL)
		Hero::image = image;
}
void Hero::Update()
{
	GameObject::Update();
	if(x < 0+frameWidth/2)
		x = 0+frameWidth/2;
	else if(x > WIDTH-frameWidth/2)
		x = WIDTH-frameWidth/2;
	if(y < 0+frameHeight/2)
		y = 0+frameHeight/2;
	else if(y > HEIGHT-frameHeight/2)
		y = HEIGHT-frameHeight/2;
}
void Hero::Render()
{
	GameObject::Render();

	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = animationRow * frameHeight;

	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight,
		x - frameWidth / 2, y - frameHeight / 2, 0);
}

void Hero::Move(int dir)
{
	if(dir == 0)
	{
		animationRow = 0;
		dirY = -1;
		if(framecount <= animationDelay)
		{
			curFrame++;
			if(curFrame = 2)
				curFrame = 0;
		}
		else
			framecount = 1;
		framecount++;
	}
	else if(dir == 2)
	{
		animationRow = 3;
		dirY = 1;
		if(framecount <= animationDelay)
		{
			curFrame++;
			if(curFrame = 2)
				curFrame = 0;
		}
		else
			framecount = 1;
		framecount++;
	}
	if(dir == 3)
	{
		animationRow = 2;
		dirX = -1;
		if(framecount <= animationDelay)
		{
			curFrame++;
			if(curFrame = 2)
				curFrame = 0;
		}
		else
			framecount = 1;
		framecount++;
	}
	else if(dir == 1)
	{
		animationRow = 1;
		dirX = 1;
		if(framecount <= animationDelay)
		{	
			curFrame++;
			if(curFrame = 2)
				curFrame = 0;	
		}
		else
			framecount = 1;
		framecount++;
	}
}

void Hero::Stop(int flag)
{
	if (flag == 0)
	{
		dirY = 0;
	}
	else
	{
		dirX = 0;
	}
}

void Hero::Collided(int objectID)
{
	if(objectID == MOB)
		lives--;
}