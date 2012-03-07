#include "Background.h"

Background::Background(ALLEGRO_BITMAP *image, float velX)
{
	GameObject::Init(0, 0, velX, 0, -1, 0, 0, 0);

	SetID(MISC);
	SetCollidable(false);

	Background::image = image;
	frameWidth = al_get_bitmap_width(image);
}
void Background::Destroy()
{
	GameObject::Destroy();
}

void Background::Update()
{
	GameObject::Update();
	if(x + frameWidth <= 0)
		x = 0;
}
void Background::Render()
{
	GameObject::Render();
	al_draw_bitmap(image, x, y, 0);

	if(x + frameWidth < WIDTH)
		al_draw_bitmap(image, x + frameWidth, y, 0);
}