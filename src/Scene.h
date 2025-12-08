#pragma once

#include "Module.h"
#include "Player.h"
#include "UIButton.h"

struct SDL_Texture;

//L17 TODO 1: Define SceneID enum class with INTRO_SCREEN, MAIN_MENU, LEVEL1, LEVEL2

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Return the player position
	Vector2D GetPlayerPosition();

	// Get tilePosDebug value
	std::string GetTilePosDebug() {
		return tilePosDebug;
	}

	// Handles multiple Gui Event methods
	bool OnUIMouseClickEvent(UIElement* uiElement);

	// L17 TODO 2: Define functions to handle scene changes

private:

	// L17 TODO 3: Define specific function for main menu scene: Load, Unload, Handle UI events

	// L17 TODO 4: Define specific functions for level1 scene: Load, Unload, Update, PostUpdate

	// L17 TODO 5: Define specific functions for level2 scene: Load, Unload, Update

private:

	//L03: TODO 3b: Declare a Player attribute
	std::shared_ptr<Player> player;
	SDL_Texture* mouseTileTex = nullptr;
	std::string tilePosDebug = "[0,0]";
	bool once = false;

	// L16: TODO 2: Declare a UIButton 
	std::shared_ptr<UIButton> uiBt;

	float volume = 1.0;

	// L17 TODO 1: Current scene attribute with initial value

};