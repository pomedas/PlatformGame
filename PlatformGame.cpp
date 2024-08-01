// PlatformGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL2/SDL.h>

// Constants for the screen width and height.
const int screenWidth = 1280;
const int screenHeight = 720;

int main(int argc, char* argv[]) {

	// Initialize the SDL video subsystem (returns 0 on success).
	if (SDL_Init(SDL_INIT_VIDEO)) {
		// Log an error message if SDL fails to initialize and exit
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		return 3; // Return 3 to indicate an error occurred.
	}

	// Create an SDL window: centered with the title 'Hello, SDL2!' and resizable	
	SDL_Window* window = SDL_CreateWindow("Hello, SDL2!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_RESIZABLE);

	if (!window) { // If the window could not be created, log the error and exit
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
		return 3;
	}

	// Create a renderer for the window created above with hardware acceleration and VSync enabled.
	// VSync is a feature that prevents screen tearing by synchronizing the frame rate with the monitor's refresh rate.
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// If the rendered could not be created, log the error and exit.
	if (!renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError());
		return 3;
	}

	// Query the current window size
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	// Define a 100x100 pixel sqaure: { x, y, width, height }
	// The squares's location will start at a random position on the screen.
	int squareSize = 100;
	SDL_Rect square{ 100,
					  100,
					  squareSize,
					  squareSize };

	// Our main loop will continue to execute until running is set to false.
	bool running{ true };
	// This flag will be set to true if the user presses a key or clicks the mouse.
	bool userInput{ false };

	// The main application loop.
	while (running) {
		SDL_Event event;

		// Loop until there are no more pending events to process. 
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				running = false; // Stop running if the window is closed
			}
			else if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
				userInput = true;
			}
			else if (event.type == SDL_KEYUP || event.type == SDL_MOUSEBUTTONUP) {
				userInput = false;
			}
		}

		// Get the window size again, in case it has been resized by the user.
		SDL_GetWindowSize(window, &w, &h);

		// Clear the screen to black.
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);

		// Draw in green if the user is pressing a key or mouse button.
		if (userInput) {
			SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
		}
		else { // Draw in blue if there is no user input. 
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x00);
		}

		// Draw the rectangle.
		SDL_RenderFillRect(renderer, &square);

		// Render everything to the screen.
		SDL_RenderPresent(renderer);

	}

	// Cleanup SDL components and quit
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

    return 0;
}
