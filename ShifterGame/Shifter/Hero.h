#pragma once

#include "GameObject.h"

class Hero : public GameObject
{
private:
	int lives;
	int animationRow;
	int framecount;

public:
	Hero();
	void Destroy();

	void Init(ALLEGRO_BITMAP *image = NULL);
	void Update();
	void Render();

	void Move(int dir);
	void Stop(int flag);

	int GetLives() { return lives; }

	void LoseLife() { lives--; }

	void Collided(int objectID);
};