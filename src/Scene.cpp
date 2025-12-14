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
#include "UIManager.h"

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
	LoadScene(currentScene); // empieza en MAIN_MENU
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
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
	switch (currentScene)
	{
	case SceneID::INTRO_SCREEN:
		break;
	case SceneID::MAIN_MENU:
		UpdateMainMenu(dt);
		break;
	case SceneID::LEVEL1:
		UpdateLevel1(dt);
		break;
	case SceneID::LEVEL2:
		UpdateLevel2(dt);
		break;
	}

	return true;
}

bool Scene::PostUpdate()
{
	bool ret = true;

	switch (currentScene)
	{
	case SceneID::INTRO_SCREEN:
		break;
	case SceneID::MAIN_MENU:
		break;
	case SceneID::LEVEL1:
		PostUpdateLevel1();
		break;
	case SceneID::LEVEL2:
		break;
	default:
		break;
	}

	if(Engine::GetInstance().input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool Scene::OnUIMouseClickEvent(UIElement* uiElement)
{
	switch (currentScene)
	{
	case SceneID::INTRO_SCREEN:
		break;
	case SceneID::MAIN_MENU:
		HandleMainMenuUIEvents(uiElement);
		break;
	case SceneID::LEVEL1:
		break;
	case SceneID::LEVEL2:
		break;
	default:
		break;
	}

	return true;
}

bool Scene::CleanUp()
{
	LOG("Freeing scene");
	UnloadCurrentScene();
	return true;
}

Vector2D Scene::GetPlayerPosition()
{
	if (player) return player->GetPosition();
	else Vector2D(0,0);
}

// *********************************************
// Scene change functions
// *********************************************

void Scene::LoadScene(SceneID newScene)
{
	auto& engine = Engine::GetInstance();

	switch (newScene)
	{
	case SceneID::MAIN_MENU:
		LoadMainMenu();
		break;

	case SceneID::LEVEL1:
		LoadLevel1();
		break;

	case SceneID::LEVEL2:
		LoadLevel2();
		break;
	}
}

void Scene::ChangeScene(SceneID newScene)
{
	UnloadCurrentScene();
	currentScene = newScene;
	LoadScene(currentScene);
}

void Scene::UnloadCurrentScene() {

	switch (currentScene)
	{
	case SceneID::MAIN_MENU:
		UnloadMainMenu();
		break;

	case SceneID::LEVEL1:
		UnloadLevel1();
		break;

	case SceneID::LEVEL2:
		UnloadLevel2();
		break;
	}

}

// *********************************************
// MAIN MENU functions
// *********************************************

void Scene::LoadMainMenu() {

	Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/retro-gaming-short-248416.wav");

	// Instantiate a UIButton in the Scene
	SDL_Rect btPos = { 520, 350, 120,20 };
	std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 1, "MyButton", btPos, this));
}

void Scene::UnloadMainMenu() {
	// Clean up UI elements related to the main menu
	Engine::GetInstance().uiManager->CleanUp();	
}

void Scene::UpdateMainMenu(float dt) {}

void Scene::HandleMainMenuUIEvents(UIElement* uiElement)
{
	switch (uiElement->id)
	{
	case 1: // Button MyButton
		LOG("Main Menu: MyButton clicked!");
		ChangeScene(SceneID::LEVEL1);
		break;
	default:
		break;
	}
}

// *********************************************
// Level 1 functions
// *********************************************

void Scene::LoadLevel1() {

	Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/level-iv-339695.wav");

	//Call the function to load the map. 
	Engine::GetInstance().map->Load("Assets/Maps/", "MapTemplate.tmx");

	//Call the function to load entities from the map
	Engine::GetInstance().map->LoadEntities(player);

	//Create a new item using the entity manager and set the position to (200, 672) to test
	std::shared_ptr<Item> item = std::dynamic_pointer_cast<Item>(Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM));
	item->position = Vector2D(200, 672);
	item->Start(); //L17 Important call Start

	//Create a new enemy 
	std::shared_ptr<Enemy> enemy1 = std::dynamic_pointer_cast<Enemy>(Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY));
	enemy1->position = Vector2D(384, 672);
	enemy1->Start(); //L17 Important call Start

	//Create a new enemy 
	std::shared_ptr<Enemy> enemy2 = std::dynamic_pointer_cast<Enemy>(Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY));
	enemy2->position = Vector2D(1380, 672);
	enemy2->Start(); //L17 Important call Start
}

void Scene::UpdateLevel1(float dt) {

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		ChangeScene(SceneID::LEVEL2);
	}

}

void Scene::UnloadLevel1() {

	// Clean up UI elements related to the Level1
	auto& uiManager = Engine::GetInstance().uiManager;
	uiManager->CleanUp();

	// Reset player reference (sets the shared_ptr to nullptr)
	player.reset();

	// Clean up map and entities
	Engine::GetInstance().map->CleanUp();
	Engine::GetInstance().entityManager->CleanUp();

}

void  Scene::PostUpdateLevel1() {

	//L15 TODO 3: Call the function to load entities from the map
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		Engine::GetInstance().map->LoadEntities(player);
	}

	//L15 TODO 4: Call the function to save entities from the map
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		Engine::GetInstance().map->SaveEntities(player);
	}
}

// *********************************************
// Level 2 functions
// *********************************************

void Scene::LoadLevel2() {

	Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/that-8-bit-music-322062.wav");

	//Call the function to load the map. 
	Engine::GetInstance().map->Load("Assets/Maps/", "MapTemplateLevel2.tmx");

	//Call the function to load entities from the map
	Engine::GetInstance().map->LoadEntities(player);
}

void Scene::UpdateLevel2(float dt) {
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		ChangeScene(SceneID::LEVEL1);
	}
}

void Scene::UnloadLevel2() {

	// Clean up UI elements related to the Level2
	auto& uiManager = Engine::GetInstance().uiManager;
	uiManager->CleanUp();

	// Reset player reference (sets the shared_ptr to nullptr)
	player.reset();

	// Clean up map and entities
	Engine::GetInstance().map->CleanUp();
	Engine::GetInstance().entityManager->CleanUp();

}

