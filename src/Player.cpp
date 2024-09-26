#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name = "Player";
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: TODO 2: Initialize Player parameters

	return true;
}

bool Player::Start() {

	//L03: TODO 2: Initialize Player parameters
	return true;
}

bool Player::Update(float dt)
{
	//L03: TODO 4: render the player texture and modify the position of the player using WSAD keys and render the texture

	return true;
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().textures.get()->UnLoad(texture);
	return true;
}