#pragma once

#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "globals.h"

class GameObject
{
private:
	int ID;
	bool alive;
	bool collidable;

protected:
	float x;
	float y;

	float velX;
	float velY;

	int dirX;
	int dirY;

	int boundX;
	int boundY;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;
	int animationDelay;

	ALLEGRO_BITMAP *image;

public:
	GameObject();
	void virtual Destroy();

	void Init(float x, float y, float velX, float velY, int dirX, int dirY, int boundX, int boundY);
	void virtual Update();
	void virtual Render();

	float GetX() { return x; }
	float GetY() { return y; }
	
	void SetX(float x) { GameObject::x = x; }
	void SetY(float y) { GameObject::y = y; }

	int GetBoundX() { return boundX; }
	int GetBoundY() { return boundY; }

	int GetID() { return ID; }
	void SetID(int ID) { GameObject::ID = ID; }

	bool GetAlive() { return alive; }
	void SetAlive(bool alive) { GameObject::alive = alive; }

	bool GetCollidable() { return collidable; }
	void SetCollidable(bool collidable) { GameObject::collidable = collidable; }

	bool CheckCollisions(GameObject *otherObject);
	void virtual Collided(int objectID);
	bool Collidable();
};