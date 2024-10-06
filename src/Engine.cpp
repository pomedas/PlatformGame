#include "Engine.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Log.h"

#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"

// Constructor
Engine::Engine() {

	LOG("Constructor Engine::Engine");

    //Measure the amount of ms that takes to execute the App constructor and LOG the result
    Timer timer = Timer();
    startupTime = Timer();
    frameTime = PerfTimer();
    lastSecFrameTime = PerfTimer();
    frames = 0;

    // L4: TODO 1: Add the EntityManager Module to the Engine
    
    // Modules
    window = std::make_shared<Window>();
    input = std::make_shared<Input>();
    render = std::make_shared<Render>();
    textures = std::make_shared<Textures>();
    audio = std::make_shared<Audio>();
    // L08: TODO 2: Add Physics module
    physics = std::make_shared<Physics>();
    scene = std::make_shared<Scene>();
    map = std::make_shared<Map>();
    entityManager = std::make_shared<EntityManager>();

    // Ordered for awake / Start / Update
    // Reverse order of CleanUp
    AddModule(std::static_pointer_cast<Module>(window));
    AddModule(std::static_pointer_cast<Module>(input));
    AddModule(std::static_pointer_cast<Module>(textures));
    AddModule(std::static_pointer_cast<Module>(audio));
    // L08: TODO 2: Add Physics module
    AddModule(std::static_pointer_cast<Module>(physics));
    AddModule(std::static_pointer_cast<Module>(map));
    AddModule(std::static_pointer_cast<Module>(scene));
    AddModule(std::static_pointer_cast<Module>(entityManager));

    // Render last 
    AddModule(std::static_pointer_cast<Module>(render));

    LOG("Timer App Constructor: %f", timer.ReadMSec());
}

// Static method to get the instance of the Engine class, following the singletn pattern
Engine& Engine::GetInstance() {
    static Engine instance; // Guaranteed to be destroyed and instantiated on first use
    return instance;
}

void Engine::AddModule(std::shared_ptr<Module> module){
    module->Init();
    moduleList.push_back(module);
}

// Called before render is available
bool Engine::Awake() {

    //Measure the amount of ms that takes to execute the Awake and LOG the result
    Timer timer = Timer();

    LOG("Engine::Awake");

    //L05 TODO 2: Add the LoadConfig() method here
    LoadConfig();

    // L05: TODO 3: Read the title from the config file and set the variable gameTitle, read maxFrameDuration and set the variable
    // also read maxFrameDuration 
    gameTitle = configFile.child("config").child("engine").child("title").child_value();
    maxFrameDuration = configFile.child("config").child("engine").child("maxFrameDuration").attribute("value").as_int();

    //Iterates the module list and calls Awake on each module
    bool result = true;
    for (const auto& module : moduleList) {
        module.get()->LoadParameters(configFile.child("config").child(module.get()->name.c_str()));
        result =  module.get()->Awake();
        if (!result) {
			break;
		}
    }

    LOG("Timer App Awake(): %f", timer.ReadMSec());

    return result;
}

// Called before the first frame
bool Engine::Start() {

    //Measure the amount of ms that takes to execute the App Start() and LOG the result
    Timer timer = Timer();

    LOG("Engine::Start");

    //Iterates the module list and calls Start on each module
    bool result = true;
    for (const auto& module : moduleList) {
        result = module.get()->Start();
        if (!result) {
            break;
        }
    }

    LOG("Timer App Start(): %f", timer.ReadMSec());

    return result;
}

// Called each loop iteration
bool Engine::Update() {

    bool ret = true;
    PrepareUpdate();

    if (input->GetWindowEvent(WE_QUIT) == true)
        ret = false;

    if (ret == true)
        ret = PreUpdate();

    if (ret == true)
        ret = DoUpdate();

    if (ret == true)
        ret = PostUpdate();

    FinishUpdate();
    return ret;
}

// Called before quitting
bool Engine::CleanUp() {

    //Measure the amount of ms that takes to execute the App CleanUp() and LOG the result
    Timer timer = Timer();

    LOG("Engine::CleanUp");

    //Iterates the module list and calls CleanUp on each module
    bool result = true;
    for (const auto& module : moduleList) {
        result = module.get()->CleanUp();
        if (!result) {
            break;
        }
    }

    LOG("Timer App CleanUp(): %f", timer.ReadMSec());

    return result;
}

// ---------------------------------------------
void Engine::PrepareUpdate()
{
    frameTime.Start();
}

// ---------------------------------------------
void Engine::FinishUpdate()
{
    // L03: TODO 1: Cap the framerate of the gameloop
    double currentDt = frameTime.ReadMs();
    if (maxFrameDuration > 0 && currentDt < maxFrameDuration) {
        int delay = (int)(maxFrameDuration - currentDt);

        // L03: TODO 2: Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
        PerfTimer delayTimer = PerfTimer();
        SDL_Delay(delay);
        //Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
        //LOG("We waited for %I32u ms and got back in %f ms",delay,delayTimer.ReadMs()); // Uncomment this line to see the results
    }

    // Amount of frames since startup
    frameCount++;

    // Amount of time since game start (use a low resolution timer)
    secondsSinceStartup = startupTime.ReadSec();

    // Amount of ms took the last update (dt)
    dt = (float)frameTime.ReadMs();

    // Amount of frames during the last second
    lastSecFrameCount++;

    // Average FPS for the whole game life
    if (lastSecFrameTime.ReadMs() > 1000) {
        lastSecFrameTime.Start();
        averageFps = (averageFps + lastSecFrameCount) / 2;
        framesPerSecond = lastSecFrameCount;
        lastSecFrameCount = 0;
    }

    // Shows the time measurements in the window title
    // check sprintf formats here https://cplusplus.com/reference/cstdio/printf/
    std::stringstream ss;
    ss << gameTitle << ": Av.FPS: " << std::fixed << std::setprecision(2) << averageFps
        << " Last sec frames: " << framesPerSecond
        << " Last dt: " << std::fixed << std::setprecision(3) << dt
        << " Time since startup: " << secondsSinceStartup
        << " Frame Count: " << frameCount;

    std::string titleStr = ss.str();

    window.get()->SetTitle(titleStr.c_str());
}

// Call modules before each loop iteration
bool Engine::PreUpdate()
{
    //Iterates the module list and calls PreUpdate on each module
    bool result = true;
    for (const auto& module : moduleList) {
        result = module.get()->PreUpdate();
        if (!result) {
            break;
        }
    }

    return result;
}

// Call modules on each loop iteration
bool Engine::DoUpdate()
{
    //Iterates the module list and calls Update on each module
    bool result = true;
    for (const auto& module : moduleList) {
        result = module.get()->Update(dt);
        if (!result) {
            break;
        }
    }

    return result;
}

// Call modules after each loop iteration
bool Engine::PostUpdate()
{
    //Iterates the module list and calls PostUpdate on each module
    bool result = true;
    for (const auto& module : moduleList) {
        result = module.get()->PostUpdate();
        if (!result) {
            break;
        }
    }

    return result;
}

// Load config from XML file
bool Engine::LoadConfig()
{
    bool ret = true;

    // L05: TODO 2: Load config.xml file using load_file() method from the xml_document class
    // If the result is ok get the main node of the XML
    // else, log the error
    // check https://pugixml.org/docs/quickstart.html#loading

    pugi::xml_parse_result result = configFile.load_file("config.xml");
    if (result)
    {
        LOG("config.xml parsed without errors");
    }
    else
    {
        LOG("Error loading config.xml: %s", result.description());
    }

    return ret;
}


