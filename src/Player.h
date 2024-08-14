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

public:

	//Declare player parameters
	float speed = 0.2f;
	SDL_Texture* texture = NULL;
};