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

	//L04: TODO 3b: Instantiate the player using the entity manager
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
	player->SetParameters(configParameters.child("entities").child("player"));
	
	//L08 Create a new item using the entity manager and set the position to (200, 672) to test
	for(pugi::xml_node itemNode = configParameters.child("entities").child("items").child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*) Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
		item->SetParameters(itemNode);
	}

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//L06 TODO 3: Call the function to load the map. 
	Engine::GetInstance().map->Load(configParameters.child("map").attribute("path").as_string(), configParameters.child("map").attribute("name").as_string());

	// Texture to highligh mouse position 
	mouseTileTex = Engine::GetInstance().textures.get()->Load("Assets/Maps/tileSelection.png");

	// Initalize the camera position in the center of the map
	int w, h;
	Engine::GetInstance().window.get()->GetWindowSize(w, h);
	Engine::GetInstance().render.get()->camera.x = w/ 2;
	Engine::GetInstance().render.get()->camera.y = 0;

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

	if(Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		Engine::GetInstance().render.get()->camera.y -= ceil(camSpeed * dt);

	if(Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		Engine::GetInstance().render.get()->camera.y += ceil(camSpeed * dt);

	if(Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		Engine::GetInstance().render.get()->camera.x -= ceil(camSpeed * dt);

	if(Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		Engine::GetInstance().render.get()->camera.x += ceil(camSpeed * dt);

	// L10 TODO 6: Implement a method that repositions the player in the map with a mouse click
	
	//Get mouse position and obtain the map coordinate
	Vector2D mousePos = Engine::GetInstance().input.get()->GetMousePosition();
	Vector2D mouseTile = Engine::GetInstance().map.get()->WorldToMap(mousePos.getX() - Engine::GetInstance().render.get()->camera.x,
																     mousePos.getY() - Engine::GetInstance().render.get()->camera.y);


	//Render a texture where the mouse is over to highlight the tile, use the texture 'mouseTileTex'
	Vector2D highlightTile = Engine::GetInstance().map.get()->MapToWorld(mouseTile.getX(),mouseTile.getY());
	Engine::GetInstance().render.get()->DrawTexture(mouseTileTex,
													highlightTile.getX() - Engine::GetInstance().map.get()->GetTileWidth() / 2,
													highlightTile.getY());

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

	if(Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	return true;
}
