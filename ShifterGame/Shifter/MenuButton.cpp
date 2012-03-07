#include "MenuButton.h"

MenuButton::MenuButton(int x, int y)
{
	SetID(MISC);
	MenuButton::SetX(x);
	MenuButton::SetY(y);
	MenuButton::setSelected(false);
	MenuButton::SetAlive(true);
	MenuButton::SetCollidable(false);
}
void MenuButton::Destroy()
{
	GameObject::Destroy();
}

void MenuButton::Update()
{
	GameObject::Update();
}
void MenuButton::Render(ALLEGRO_FONT *font, char str[80])
{
	GameObject::Render();
	if (MenuButton::isSelected())
	{
		al_draw_text(font, al_map_rgb(255, 255, 255), MenuButton::GetX(), MenuButton::GetY(), ALLEGRO_ALIGN_CENTRE, str);
	}
	else
	{
		al_draw_text(font, al_map_rgb(200, 200, 200), MenuButton::GetX(), MenuButton::GetY(), ALLEGRO_ALIGN_CENTRE, str);
	}
}