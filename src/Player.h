#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	// L08 TODO 6: Define OnCollision function for the player. 

public:

	//Declare player parameters
	float speed = 5.0f;
	SDL_Texture* texture = NULL;

	// L08 TODO 5: Add physics to the player - declare a Physics body
};