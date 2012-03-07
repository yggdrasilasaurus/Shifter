#pragma once

#include "GameObject.h"

class MenuButton : public GameObject
{
private:
	int r;
	int g;
	int b;
	bool selected;
public:
	MenuButton(int x, int y);
	void Destroy();

	void Update();
	void Render(ALLEGRO_FONT *font, char str[]);

	void Init();

	void ChangeColor(int r, int g, int b) { MenuButton::r = r; MenuButton::g = g; MenuButton::b = b; }
	int getR() { return MenuButton::r; }
	int getG() { return MenuButton::g; }
	int getB() { return MenuButton::b; }

	bool isSelected() { return selected; }
	void toggleSelected() { MenuButton::selected = !MenuButton::selected; }
	void setSelected(bool selected) { MenuButton::selected = selected; }
};