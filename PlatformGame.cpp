#include <SDL3/SDL.h>   // Include the main SDL3 header
#include <iostream>     // For printing errors to the console

// Constants for screen and cube size
constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr int CUBE_SIZE = 50;

int main(int, char**) {
    // 1. Initialize SDL with video support
    //    This sets up everything needed for graphics.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    // 2. Create the application window
    //    Parameters:
    //    - Title
    //    - Width and Height
    //    - Flags (here: resizable window)
    SDL_Window* window = SDL_CreateWindow("SDL3 Cube Example",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    // Center the window on the screen and make it visible
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // 3. Create a renderer
    //    The renderer is what we use to draw shapes and textures inside the window.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 4. Define the starting position of our cube (centered on screen)
    int cubeX = (SCREEN_WIDTH - CUBE_SIZE) / 2;
    int cubeY = (SCREEN_HEIGHT - CUBE_SIZE) / 2;

    // 5. Main loop variables
    bool running = true;   // controls when the program should stop
    SDL_Event e;           // stores events like key presses, mouse movement, etc.

    // 6. Main game loop
    //    This loop keeps running until the user closes the window.
    while (running) {
        // 6a. Handle events (input, quit, etc.)
        while (SDL_PollEvent(&e)) {  // Get events from the queue
            switch (e.type) {
            case SDL_EVENT_QUIT:     // User clicked the X button on the window
                running = false;
                break;

            case SDL_EVENT_KEY_DOWN: // User pressed a key
                switch (e.key.key) {
                case SDLK_UP:    cubeY -= 10; break;   // Move cube up
                case SDLK_DOWN:  cubeY += 10; break;   // Move cube down
                case SDLK_LEFT:  cubeX -= 10; break;   // Move cube left
                case SDLK_RIGHT: cubeX += 10; break;   // Move cube right
                }
                break;
            }
        }

        // 6b. Clear the screen with black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // RGBA: black
        SDL_RenderClear(renderer);

        // 6c. Draw the cube as a blue rectangle
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // RGBA: blue
        SDL_FRect rect = {
            static_cast<float>(cubeX),
            static_cast<float>(cubeY),
            static_cast<float>(CUBE_SIZE),
            static_cast<float>(CUBE_SIZE)
        };
        SDL_RenderFillRect(renderer, &rect); // SDL3 uses SDL_FRect for drawing

        // 6d. Show everything we drew this frame
        SDL_RenderPresent(renderer);
    }

    // 7. Cleanup resources before exiting
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
