#include "PlayState.h"
#include "Player.h"

#include <vector>
#include <time.h>
#include <iostream>

#include "mesh.h"
#include "Game.h"

#include <allegro5\allegro_font.h>

PlayState::PlayState(ALLEGRO_DISPLAY* display) : ScreenState(display) {
	log("Loading game\n");

	map = new Map(Game::get()->getManager());
	hud = new IngameHUD();

	controller = new PlayerController(map->getEntitiesByClass<Player>().at(0));
	camera = new Camera(al_get_display_width(display), al_get_display_height(display), *controller);
}

PlayState::~PlayState(void){
	delete map;
	delete hud;
	delete controller;
	delete camera;
}

void PlayState::tick(double deltaTime){
	controller->move(deltaTime);

	camera->camera_3D_setup(getDisplay());
	map->draw(deltaTime);

	/*camera->camera_2D_setup(getDisplay());
	hud->draw(this);*/
}

void PlayState::handleKeyboard(ALLEGRO_EVENT_TYPE type, int keycode){
	// If a key is pressed, toggle the relevant key-press flag
	if (type == ALLEGRO_EVENT_KEY_DOWN){
		switch (keycode) {
		case ALLEGRO_KEY_W:
			controller->holdingForward = true;
			break;
		case ALLEGRO_KEY_S:
			controller->holdingBackward = true;
			break;
		case ALLEGRO_KEY_A:
			controller->holdingLeftStrafe = true;
			break;
		case ALLEGRO_KEY_D:
			controller->holdingRightStrafe = true;
			break;
		default:
			// Do nothing...
			break;
		}
	} else if (type == ALLEGRO_EVENT_KEY_UP) { // If a key is released, toggle the relevant key-release flag 
		switch (keycode) {
		case ALLEGRO_KEY_W:
			controller->holdingForward = false;
			break;
		case ALLEGRO_KEY_S:
			controller->holdingBackward = false;
			break;
		case ALLEGRO_KEY_A:
			controller->holdingLeftStrafe = false;
			break;
		case ALLEGRO_KEY_D:
			controller->holdingRightStrafe = false;
			break;
		default:
			// Do nothing...
			break;
		}
	}
}

void PlayState::handleMouse(ALLEGRO_EVENT_TYPE type, ALLEGRO_MOUSE_STATE state){
	float width = getDisplayWidth(), height = getDisplayHeight();
	
	if (type == ALLEGRO_EVENT_MOUSE_AXES){
		if (al_mouse_button_down(&state, 2)){
			if (mouseTempX < 0 || mouseTempY < 0){
				mouseTempX = state.x;
				mouseTempY = state.y;
			}

			controller->handleMouseMove(state.x - mouseTempX, state.y - mouseTempY);
			al_set_mouse_xy(getDisplay(), mouseTempX, mouseTempY);
		} else if (mouseTempX >= 0 || mouseTempY >= 0){
			//mouse released after dragging
			mouseTempX = -1;
			mouseTempY = -1;
		}
	}
}