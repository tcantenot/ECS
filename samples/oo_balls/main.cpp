#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>

// Managers
#include "BallManager.hpp"

// System->
#include "Systems.hpp"

// Common
#include "graphics/Initialisation.h"
#include "graphics/PixelBuffer.h"

// SDL
#include <SDL2/SDL.h>


// Thanks, Obama
#ifdef _MSC_VER
	#define snprintf _snprintf
#endif

// Window size
int width = 800;
int height = 600;

// Constants
const char* TITLE_FORMAT = "Balls: %d, FPS: %d, Rendering: %s (press R), Movement: %s (press M), right/left arrows to add/remove balls";

const int INITIAL_WIDTH = 800;
const int INITIAL_HEIGHT = 600;
const int DEPTH = 32;

const int CLEAR_COLOUR = 0xFFFFFFFF;

const int INITIAL_BALLS = 10;

// 0.001f to convert milliseconds to seconds
const float GAME_SPEED = 0.001f;

int main(int, char**)
{
	// Timing
	unsigned int lastUpdate;
	unsigned int thisUpdate;
	unsigned int dt = 0;

	// FPS
	int frames = 0;
	int fps = 0;
	int lastFPSUpdate;

	// Pixel buffer
	PixelBuffer pixelBuffer(width, height);

	// SDL structures
	SDL_Event event;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* renderTexture;

	// Seed random number generator
	srand((unsigned int)time(0));

	// Initialise SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Initilialise timing
	lastUpdate = thisUpdate = SDL_GetTicks();
	lastFPSUpdate = lastUpdate;

	// Create window
	window = createWindow(TITLE_FORMAT, width, height);

	// Create renderer
	renderer = createRenderer(window);

	// Create render texture
	renderTexture = createTexture(renderer, width, height);

	// Set window values
    //int width  = INITIAL_WIDTH;
    //int height = INITIAL_HEIGHT;

	// Create and initialise managers
	BallManager ballManager(width, height);

	// Create some balls
	ballManager.createBalls(INITIAL_BALLS);

	// Start main loop
	bool running = true;
	bool rendering_enabled = true;
    bool movement_enabled  = true;

	while (running)
	{
		const SDL_Rect screenRect = {0, 0, width, height};

		// Update timer
		thisUpdate = SDL_GetTicks();
		dt = thisUpdate - lastUpdate;

		// Handle all events
		while (SDL_PollEvent(&event))
		{
			// End when the user closes the window or presses esc
			if (event.type == SDL_QUIT ||
				(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
			{
				running = false;
			}

			// Handle keyboard input
			if (event.type == SDL_KEYDOWN)
			{
				// Toggle rendering when player presses R
				if (event.key.keysym.sym == SDLK_r)
				{
					// Disable rendering
					rendering_enabled = !rendering_enabled;

					// Clear screen to white
                    pixelBuffer.clear(0xFFFFFFFF);

					// Update render texture
					SDL_UpdateTexture(renderTexture, &screenRect, pixelBuffer.getBuffer(), width * 4);

					// Render texture to screen
					SDL_RenderCopy(renderer, renderTexture, &screenRect, &screenRect);

					// Flip screen buffer
					SDL_RenderPresent(renderer);
				}
				// Toggle movement when player presses M
				else if (event.key.keysym.sym == SDLK_m)
				{
                    movement_enabled = !movement_enabled;
				}
				// Add 10 balls when user presses right arrow
				else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
				{
					ballManager.createBalls(100);
				}
				// Remove 10 balls when user presses left arrow
				else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
				{
					ballManager.destroyBalls(100);
				}
			}
		}

		// Update game
        float delta = dt*GAME_SPEED;

        if(movement_enabled)
        {
            process_collisions(width, height, ballManager.balls());
            process_movement(delta, ballManager.balls());
            process_gravity(delta, ballManager.balls());
        }

        // Clear window
        SDL_RenderClear(renderer);

		if (rendering_enabled)
		{
			// Clear buffer
            pixelBuffer.clear(CLEAR_COLOUR);

			// Run rendering system
            process_rendering(&pixelBuffer, ballManager.balls());

			// Blit buffer to screen renderer
			SDL_UpdateTexture(renderTexture, &screenRect, pixelBuffer.getBuffer(), width * 4);

			// Render texture to screen
			SDL_RenderCopy(renderer, renderTexture, &screenRect, &screenRect);

			// Flip screen buffer
			SDL_RenderPresent(renderer);
		}

		// Update last time
		lastUpdate = thisUpdate;

		// Average FPS calculations
		frames++;
		if (thisUpdate - lastFPSUpdate >= 1000)
		{
			// Update FPS counters
			fps = frames;
			frames = 0;
			lastFPSUpdate = thisUpdate;
		}

		// Update window title
		{
			const int titleBufferLen = 256;
			char titleBuffer[titleBufferLen];

			// Format window title
			const char* renderingEnabled = (rendering_enabled ? "Enabled" : "Disabled");
			const char* movementEnabled = (true ? "Enabled" : "Disabled");
			snprintf(titleBuffer, titleBufferLen, TITLE_FORMAT, ballManager.ballCount(), fps, renderingEnabled, movementEnabled);

			// Set window title
			SDL_SetWindowTitle(window, titleBuffer);
		}
	}

	// Clean up
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
