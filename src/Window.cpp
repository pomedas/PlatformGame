#include "Window.h"
#include "Log.h"
#include "Engine.h"

Window::Window() : Module()
{
	window = NULL;
	name = "window";
}

// Destructor
Window::~Window()
{
}

// Called before render is available
bool Window::Awake()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{

		// Create window
		Uint32 flags = SDL_WINDOW_SHOWN;
		if(fullscreen == true) flags |= SDL_WINDOW_FULLSCREEN;
		if(borderless == true) flags |= SDL_WINDOW_BORDERLESS;
		if(resizable == true) flags |= SDL_WINDOW_RESIZABLE;
		if(fullscreen_window == true) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
	}

	return ret;
}

// Called before quitting
bool Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	// Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	// Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Set new window title
void Window::SetTitle(const char* new_title)
{
	SDL_SetWindowTitle(window, new_title);
}

void Window::GetWindowSize(int& width, int& height) const
{
	width = this->width;
	height = this->height;
}

int Window::GetScale() const
{
	return scale;
}

bool Window::LoadParameters(xml_node parameters)
{
	bool ret = true;

	// Load all required configurations from config.xml
	// Tip: get the name of the child and the attribute value
	// Get values of fullscreen, borderless, resizable,fullscreen_window, width, height and scale from config files
	fullscreen = parameters.child("fullscreen").attribute("value").as_bool(false);
	borderless = parameters.child("borderless").attribute("value").as_bool(false);
	resizable = parameters.child("resizable").attribute("value").as_bool(false);
	fullscreen_window = parameters.child("fullscreen_window").attribute("value").as_bool(false);

	width = parameters.child("resolution").attribute("width").as_int(1280);
	height = parameters.child("resolution").attribute("height").as_int(720);
	scale = parameters.child("resolution").attribute("scale").as_int(1);

	return ret;
}