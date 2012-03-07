//include core Allegro libs and Misc libs
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <list>
#include <limits.h>
#include "globals.h"

//include entity classes
#include "GameObject.h"
#include "Background.h"
#include "MenuButton.h"
#include "Hero.h"
//globals
bool keys[] = {false, false, false, false, false, false, false};
enum KEYS{UP, DOWN, LEFT, RIGHT, SPACE, F1, ENTER};

Hero *hero;
std::list<GameObject *> objects;
std::list<GameObject *>::iterator iter;
std::list<GameObject *>::iterator iter2;

std::list<MenuButton *> menuoptions;
std::list<MenuButton *>::iterator moiter;

Background *titleScreen;
MenuButton *newGame;
MenuButton *loadGame;
MenuButton *options;
MenuButton *mapeditor;
MenuButton *quitGame;

//prototypes
void __cdecl TakeLife();
void ChangeState(int &state, int newState);
void fadeOut();

int main(int argc, char **argv)
{
	//shell variables
	bool done = false;
	bool render = false;
	bool debug = false;
	bool mouseinit = false;
	bool wintog = false;
	bool fadeout = false;
	float timeSinceLast = LONG_MAX;

	float gameTime = 0;
	int frames = 0;
	int gameFPS = 0;

	float mX;
	float mY;
	
	//project variables
	int state = -1;
	hero = new Hero();

	ALLEGRO_BITMAP *heroImage = NULL;
	ALLEGRO_BITMAP *titleImage = NULL;
	ALLEGRO_SAMPLE *sfx_select = NULL;
	char newGameText[] = "New Game";
	char loadGameText[] = "Load Game";
	char optionsText[] = "Options";
	char mapeditorText[] = "Map Editor";
	char quitGameText[] = "Quit Game";
	//allegro variables
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_FONT *matrix18;
	ALLEGRO_FONT *matrix24;
	ALLEGRO_FONT *matrix36;

	//allegro init functions
	if(!al_init())
		return -1;
	display = al_create_display(WIDTH, HEIGHT);
	if(!display)
		return -1;
	al_set_window_title(display, "Shifter v.0.01");
	//addon install
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_install_mouse();

	//project init
	al_reserve_samples(15);

	matrix18 = al_load_ttf_font("font/polix_mono.ttf", 18, 0);
	matrix24 = al_load_ttf_font("font/polix_mono.ttf", 24, 0);
	matrix36 = al_load_ttf_font("font/polix_mono.ttf", 36, 0);

	heroImage = al_load_bitmap("img/hero.png");
	hero->Init(heroImage);
	objects.push_back(hero);

	titleImage = al_load_bitmap("img/titleback.png");
	titleScreen = new Background(titleImage, 0);

	newGame = new MenuButton(WIDTH/2, HEIGHT/2);
	loadGame = new MenuButton(WIDTH/2, HEIGHT/2+25);
	options = new MenuButton(WIDTH/2, HEIGHT/2+50);
	mapeditor = new MenuButton(WIDTH/2, HEIGHT/2+75);
	quitGame = new MenuButton(WIDTH/2, HEIGHT/2+100);

	menuoptions.push_back(newGame);
	menuoptions.push_back(loadGame);
	menuoptions.push_back(options);
	menuoptions.push_back(mapeditor);
	menuoptions.push_back(quitGame);

	sfx_select = al_load_sample("sfx/select.wav");

	ChangeState(state, TITLE);
	newGame->setSelected(true);

	srand(time(NULL));

	//timer init
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_start_timer(timer);
	gameTime = al_current_time();

	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		//INPUT
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			done = true;
		else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mX = ev.mouse.x;
			mY = ev.mouse.y;
			//mouse collision
			if(state == TITLE)
			{
				if(mX >= WIDTH/2-130 && mX <= WIDTH/2+130)
				{
					if(mY >= HEIGHT/2 && mY <= HEIGHT/2+24)
					{
						for(moiter = menuoptions.begin(); moiter != menuoptions.end(); ++moiter)
						{
							if((*moiter)->isSelected())
								(*moiter)->toggleSelected();
						}
						newGame->setSelected(true);
					}
					else if(mY >= HEIGHT/2+25 && mY <= HEIGHT/2+49)
					{
						for(moiter = menuoptions.begin(); moiter != menuoptions.end(); ++moiter)
						{
							if((*moiter)->isSelected())
								(*moiter)->toggleSelected();
						}
						loadGame->setSelected(true);
					}
					else if(mY >= HEIGHT/2+50 && mY <= HEIGHT/2+74)
					{
						for(moiter = menuoptions.begin(); moiter != menuoptions.end(); ++moiter)
						{
							if((*moiter)->isSelected())
								(*moiter)->toggleSelected();
						}
						options->setSelected(true);
					}
					else if(mY >= HEIGHT/2+75 && mY <= HEIGHT/2+99)
					{
						for(moiter = menuoptions.begin(); moiter != menuoptions.end(); ++moiter)
						{
							if((*moiter)->isSelected())
								(*moiter)->toggleSelected();
						}
						mapeditor->setSelected(true);
					}
					else if(mY >= HEIGHT/2+100 && mY <= HEIGHT/2+124)
					{
						for(moiter = menuoptions.begin(); moiter != menuoptions.end(); ++moiter)
						{
							if((*moiter)->isSelected())
								(*moiter)->toggleSelected();
						}
						quitGame->setSelected(true);
					}
				}
			}
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if(state == TITLE)
			{
				if(ev.mouse.button & 1)
				{
					if(mY >= HEIGHT/2 && mY <= HEIGHT/2+24)
						ChangeState(state, INGAME);
					else if(mY >= HEIGHT/2+25 && mY <= HEIGHT/2+49)
						ChangeState(state, INGAME);
					else if(mY >= HEIGHT/2+50 && mY <= HEIGHT/2+74)
						ChangeState(state, OPTIONS);
					else if(mY >= HEIGHT/2+75 && mY <= HEIGHT/2+99)
						ChangeState(state, MAPEDIT);
					else if(mY >= HEIGHT/2+100 && mY <= HEIGHT/2+124)
						done = true;
				}
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				if(state == TITLE)
					done = true;
				else if(state == INGAME)
					ChangeState(state, INGAMEMENU);
				else if(state == INGAMEMENU)
					ChangeState(state, INGAME);
				else if(state == MAPEDIT)
					ChangeState(state, TITLE);
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				if(al_current_time() - LONG_MAX < 0.5)
					timeSinceLast = al_current_time();
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				if(al_current_time() - LONG_MAX < 0.5)
					timeSinceLast = al_current_time();
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				break;
			case ALLEGRO_KEY_ENTER:
				keys[ENTER] = true;
				break;
			case ALLEGRO_KEY_F1:
				debug = !debug;
				break;
			case ALLEGRO_KEY_F2:
				al_toggle_display_flag(display, ALLEGRO_NOFRAME, wintog);
				wintog = !wintog;
				break;
			case ALLEGRO_KEY_1:
				ChangeState(state, TITLE);
				break;
			case ALLEGRO_KEY_2:
				ChangeState(state, INGAME);
				break;
			case ALLEGRO_KEY_3:
				ChangeState(state, GAMEOVER);
				break;
			case ALLEGRO_KEY_4:
				ChangeState(state, MAPEDIT);
				break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			case ALLEGRO_KEY_ENTER:
				keys[ENTER] = false;
				break;
			}
		}
		//game update
		else if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			render = true;
			//fps update
			frames++;
			if(al_current_time() - gameTime >= 1)
			{
				gameTime = al_current_time();
				gameFPS = frames;
				frames = 0;
			}
			if(state == TITLE)
			{
				if(al_current_time() - timeSinceLast < 0.5)
				{
					if(keys[UP])
					{
						if(newGame->isSelected())
						{
							newGame->toggleSelected();
							quitGame->toggleSelected();
							al_play_sample(sfx_select, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
							timeSinceLast = 0;
						}
						else if(loadGame->isSelected())
						{
							loadGame->toggleSelected();
							newGame->toggleSelected();
							al_play_sample(sfx_select, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
							timeSinceLast = 0;
						}
						else if(options->isSelected())
						{
							options->toggleSelected();
							loadGame->toggleSelected();
							al_play_sample(sfx_select, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
							timeSinceLast = 0;
						}
						else if(mapeditor->isSelected())
						{
							mapeditor->toggleSelected();
							options->toggleSelected();
							al_play_sample(sfx_select, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
							timeSinceLast = 0;
						}
						else if(quitGame->isSelected())
						{
							quitGame->toggleSelected();
							mapeditor->toggleSelected();
							al_play_sample(sfx_select, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
							timeSinceLast = 0;
						}
					}
					else if(keys[DOWN])
					{

						if(newGame->isSelected())
						{
							newGame->toggleSelected();
							loadGame->toggleSelected();
							al_play_sample(sfx_select, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
							timeSinceLast = 0;
						}
						else if(loadGame->isSelected())
						{
							loadGame->toggleSelected();
							options->toggleSelected();
							al_play_sample(sfx_select, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
							timeSinceLast = 0;
						}
						else if(options->isSelected())
						{
							options->toggleSelected();
							mapeditor->toggleSelected();
							al_play_sample(sfx_select, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
							timeSinceLast = 0;
						}
						else if(mapeditor->isSelected())
						{
							mapeditor->toggleSelected();
							quitGame->toggleSelected();
							al_play_sample(sfx_select, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
							timeSinceLast = 0;
						}
						else if(quitGame->isSelected())
						{
							quitGame->toggleSelected();
							newGame->toggleSelected();
							al_play_sample(sfx_select, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
							timeSinceLast = 0;
						}
					}
				}
				else if(keys[SPACE] || keys[ENTER])
				{
					if(newGame->isSelected())
						fadeout = true;
					else if(loadGame->isSelected())
						ChangeState(state, INGAME);
					else if(options->isSelected())
						ChangeState(state, INGAME);
					else if(mapeditor->isSelected())
						ChangeState(state, INGAME);
					else if(quitGame->isSelected())
						done = true;
				}
				//update
				for(moiter = menuoptions.begin(); moiter != menuoptions.end(); ++moiter)
					(*moiter)->Update();
			}
			else if(state == INGAME || state == INGAMEMENU)
			{
				if(state == INGAME)
				{
					if(keys[UP])
						hero->Move(0);
					else if(keys[DOWN])
						hero->Move(2);
					else
						hero->Stop(0);
					if(keys[LEFT])
						hero->Move(3);
					else if(keys[RIGHT])
						hero->Move(1);
					else
						hero->Stop(1);
					//other things that happen in-game go here
				}
				//update
				for(iter = objects.begin(); iter != objects.end(); ++iter)
					(*iter)->Update();

				//collisions
				for(iter = objects.begin(); iter != objects.end(); ++iter)
				{
					if( !(*iter)->Collidable() ) continue;
					for(iter2 = iter; iter2 != objects.end(); ++iter2)
					{
						if( !(*iter2)->Collidable() ) continue;
						if( (*iter)->GetID() == (*iter2)->GetID()) continue;

						if( (*iter)->CheckCollisions( (*iter2)))
						{
							(*iter)->Collided( (*iter2)->GetID());
							(*iter2)->Collided( (*iter)->GetID());
						}
					}
				}
			if(hero->GetLives() <= 0)
			ChangeState(state, GAMEOVER);
			}
			//other state stuffs
			//cull the dead
			for(iter = objects.begin(); iter != objects.end(); )
			{
				if(! (*iter)->GetAlive())
				{
					delete (*iter);
					iter = objects.erase(iter);
				}
				else
					iter++;
			}
		}
		//render
		if(render && al_is_event_queue_empty(event_queue))
		{
			render = false;
			//begin rendering
			if(state == TITLE)
			{
				titleScreen->Render();
				if(debug)
					al_draw_filled_rectangle(0, 0, 125, 25, al_map_rgba(0,0,0,150));
				al_draw_filled_rectangle(0, HEIGHT/2-20, WIDTH, HEIGHT/2+140, al_map_rgba(0,0,0,150));
				newGame->Render(matrix24, newGameText);
				loadGame->Render(matrix24, loadGameText);
				options->Render(matrix24, optionsText);
				mapeditor->Render(matrix24, mapeditorText);
				quitGame->Render(matrix24, quitGameText);
				if(fadeout)
				{
					fadeOut();
					fadeout = false;
					ChangeState(state, INGAME);
				}
			}
			else if(state == INGAME || INGAMEMENU)
			{
				//al_draw_textf(font18, al_map_rgb(255, 0, 255), 5, 5, 0, "Player has %i lives left.", ship->GetLives());
				al_draw_text(matrix36, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "Inside the Game");
				for(iter = objects.begin(); iter != objects.end(); ++iter)
					(*iter)->Render();
			}
			else if(state == GAMEOVER)
			{
				//lostScreen->Render();
				al_draw_text(matrix36, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "Game Over");
			}
			else if(state == MAPEDIT)
			{
				al_draw_text(matrix36, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "Map Editor");
			}
			if(debug)
				al_draw_textf(matrix18, al_map_rgb(255, 255, 255), 5, 5, 0, "FPS: %i", gameFPS);	//display FPS on screen

			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}
	//Destroy objects
	for(iter = objects.begin(); iter != objects.end(); )
	{
		(*iter)->Destroy();
		delete (*iter);
		iter = objects.erase(iter);
	}
	for(moiter = menuoptions.begin(); moiter != menuoptions.end(); )
	{
		(*moiter)->Destroy();
		delete (*moiter);
		moiter = menuoptions.erase(moiter);
	}
	//Destroy misc
	al_destroy_sample(sfx_select);
	//Destroy shell objects
	al_destroy_font(matrix18);
	al_destroy_font(matrix24);
	al_destroy_font(matrix36);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);

	return 0;
}

void __cdecl TakeLife()
{
	hero->LoseLife();
}

void ChangeState(int &state, int newState)
{
	if(state == TITLE)
	{
		std::cout<<"Leaving Title\n";
		hero->Init();
	}
	else if(state == INGAME)
	{
		std::cout<<"Leaving In-Game\n";
		for(iter = objects.begin(); iter != objects.end(); ++iter)
		{
			if( (*iter)->GetID() != HERO && (*iter)->GetID() != MISC)
				(*iter)->SetAlive(false);			
		}

		//al_stop_sample_instance(songInstance);
	}
	else if(state == INGAMEMENU)
	{
		std::cout<<"Leaving In-Game Menu\n";
	}
	else if(state == GAMEOVER)
	{
		std::cout<<"Leaving Game Over\n";
	}
	else if(state == MAPEDIT)
	{
		std::cout<<"Leaving Map Edit\n";		
	}

	state = newState;

	if(state == TITLE)
	{
		std::cout<<"Entering Title\n";
	}
	else if(state == INGAME)
	{
		std::cout<<"Entering In-Game\n";
		//al_play_sample_instance(songInstance);
	}
	else if(state == INGAMEMENU)
	{
		std::cout<<"Entering In-Game Menu\n";
	}
	else if(state == GAMEOVER)
	{
		std::cout<<"Entering Game Over\n";
	}
	else if(state == MAPEDIT)
	{
		std::cout<<"Entering Map Edit\n";
	}
}

void fadeOut()
{
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
	for(int i = 0; i < 100; i++)
	 {
		 al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgba_f(0, 0, 0, 1.0 - 1.0 / (i + 1)));
		 al_flip_display();
		 al_clear_to_color(al_map_rgb(0,0,0));
	 }
}