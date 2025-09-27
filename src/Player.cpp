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
	position = Vector2D(0, 0);
	return true;
}

bool Player::Start() {

	//L03: TODO 2: Initialize Player parameters
	texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/player1.png");

	// L08 TODO 5: Add physics to the player - initialize physics body

	// L08 TODO 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method

	// L08 TODO 7: Assign collider type

	return true;
}

bool Player::Update(float dt)
{
	// L08 TODO 5: Add physics to the player - updated player position using physics

	//L03: TODO 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	float speed = 0.2f;

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		position.setY(position.getY() - speed *dt);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		position.setY(position.getY() + speed*dt);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		position.setX(position.getX() - speed*dt);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		position.setX(position.getX() + speed*dt);

	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY());
	return true;
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().textures.get()->UnLoad(texture);
	return true;
}

// L08 TODO 6: Define OnCollision function for the player. 