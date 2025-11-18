#include "Engine.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Log.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Player.h"
#include "Map.h"
#include "Item.h"
#include "Enemy.h"

Scene::Scene() : Module()
{
	name = "scene";
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	//L08: TODO 4: Create a new item using the entity manager and set the position to (200, 672) to test
	std::shared_ptr<Item> item = std::dynamic_pointer_cast<Item>(Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM));
	item->position = Vector2D(200, 672);

	//Create a new enemy 
	std::shared_ptr<Enemy> enemy1 = std::dynamic_pointer_cast<Enemy>(Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY));
	enemy1->position = Vector2D(384, 672);

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/level-iv-339695.wav");

	//L06 TODO 3: Call the function to load the map. 
	Engine::GetInstance().map->Load("Assets/Maps/", "MapTemplate.tmx");

	//L15 TODO 3: Call the function to load entities from the map
	Engine::GetInstance().map->LoadEntities(player);

	// Texture to highligh mouse position 
	mouseTileTex = Engine::GetInstance().textures->Load("Assets/Maps/MapMetadata.png");
	
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	//L03 TODO 3: Make the camera movement independent of framerate
	float camSpeed = 1;

	if(Engine::GetInstance().input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		Engine::GetInstance().render->camera.y -= (int)ceil(camSpeed * dt);

	if(Engine::GetInstance().input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		Engine::GetInstance().render->camera.y += (int)ceil(camSpeed * dt);

	if(Engine::GetInstance().input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		Engine::GetInstance().render->camera.x -= (int)ceil(camSpeed * dt);
	
	if(Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		Engine::GetInstance().render.get()->camera.x += (int)ceil(camSpeed * dt);

	//Get mouse position and obtain the map coordinate
	Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();
	Vector2D mouseTile = Engine::GetInstance().map->WorldToMap((int)(mousePos.getX() - Engine::GetInstance().render->camera.x),
															   (int)(mousePos.getY() - Engine::GetInstance().render->camera.y));

	//Render a texture where the mouse is over to highlight the tile, use the texture 'mouseTileTex'
	Vector2D highlightTile = Engine::GetInstance().map.get()->MapToWorld((int)mouseTile.getX(), (int)mouseTile.getY());
	SDL_Rect rect = { 0,0,Engine::GetInstance().map->GetTileWidth(),Engine::GetInstance().map->GetTileHeight() };
	Engine::GetInstance().render.get()->DrawTexture(mouseTileTex,(int)highlightTile.getX(),(int)highlightTile.getY(),&rect);

	// saves the tile pos for debugging purposes
	if (mouseTile.getX() >= 0 && mouseTile.getY() >= 0 || once) {
		tilePosDebug = "[" + std::to_string((int)mouseTile.getX()) + "," + std::to_string((int)mouseTile.getY()) + "] ";
		once = true;
	}

	//If mouse button is pressed modify player position
	if (Engine::GetInstance().input.get()->GetMouseButtonDown(1) == KEY_DOWN) {
		player->SetPosition(Vector2D(highlightTile.getX(), highlightTile.getY()));
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	//L15 TODO 3: Call the function to load entities from the map
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		Engine::GetInstance().map->LoadEntities(player);
	}

	//L15 TODO 4: Call the function to save entities from the map
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		Engine::GetInstance().map->SaveEntities(player);
	}

	if(Engine::GetInstance().input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

// Return the player position
Vector2D Scene::GetPlayerPosition()
{
	return player->GetPosition();
}
