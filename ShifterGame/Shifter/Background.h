#pragma once

#include "GameObject.h"

class Background : public GameObject
{
private:

public:
	Background(ALLEGRO_BITMAP *image, float velX);
	void Destroy();

	void Update();
	void Render();
};