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
	player = std::dynamic_pointer_cast<Player>(Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER));
	
	//L08: TODO 4: Create a new item using the entity manager and set the position to (200, 672) to test
	std::shared_ptr<Item> item = std::dynamic_pointer_cast<Item>(Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM));
	item->position = Vector2D(200, 672);

	return ret;
}

// Called before the first frame
bool Scene::Start()
{

	Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/level-iv-339695.wav");

	//L06 TODO 3: Call the function to load the map. 
	Engine::GetInstance().map->Load("Assets/Maps/", "MapTemplate.tmx");
	
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

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

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
