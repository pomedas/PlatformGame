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
		bool fullscreen = configParameters.child("fullscreen").attribute("value").as_bool();
		bool borderless = configParameters.child("borderless").attribute("value").as_bool();
		bool resizable = configParameters.child("resizable").attribute("value").as_bool();
		bool fullscreen_window = configParameters.child("fullscreen_window").attribute("value").as_bool();

		//TODO Get the values from the config file
		width = configParameters.child("resolution").attribute("width").as_int();
		height = configParameters.child("resolution").attribute("height").as_int();
		scale = configParameters.child("resolution").attribute("scale").as_int();

		if(fullscreen == true) flags |= SDL_WINDOW_FULLSCREEN;
		if(borderless == true) flags |= SDL_WINDOW_BORDERLESS;
		if(resizable == true) flags |= SDL_WINDOW_RESIZABLE;
		if(fullscreen_window == true) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		window = SDL_CreateWindow("Platform Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

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