#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>

#include <HybridECS.hpp>

// Managers
#include "managers/BallManager.hpp"

// System->
#include "systems/RenderingSystem.hpp"
#include "systems/CollisionSystem.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/GravitySystem.hpp"

// Common
#include "graphics/Initialisation.h"
#include "graphics/PixelBuffer.h"

// SDL
#include <SDL2/SDL.h>


// Thanks, Obama
#ifdef _MSC_VER
	#define snprintf _snprintf
#endif

using namespace Hybrid::ECS;

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

	// Create entity world
	World world;

	// Set window values
	world.setValue<int>("window_width", INITIAL_WIDTH);
	world.setValue<int>("window_height", INITIAL_HEIGHT);

	// Create and initialise systems
	auto renderingSystem = world.registerSystem<RenderingSystem>(&pixelBuffer);
	auto collisionSystem = world.registerSystem<CollisionSystem>();
	auto movementSystem  = world.registerSystem<MovementSystem>();
	auto gravitySystem   = world.registerSystem<GravitySystem>();

    renderingSystem->setEnabled(true);

	// Create and initialise managers
	auto ballManager  = world.registerManager<BallManager>(width, height);

	// Create some balls
	ballManager->createBalls(INITIAL_BALLS);

	// Start main loop
	world.setValue<bool>("running", true);
	world.setValue<bool>("rendering_enabled", true);
	while (world.getValue<bool>("running"))
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
				world.setValue<bool>("running", false);
			}

			// Handle keyboard input
			if (event.type == SDL_KEYDOWN)
			{
				// Toggle rendering when player presses R
				if (event.key.keysym.sym == SDLK_r)
				{
					// Disable rendering
					renderingSystem->setEnabled(!renderingSystem->isEnabled());
					world.setValue("rendering_enabled", renderingSystem->isEnabled());

					// Clear screen to white
                    pixelBuffer.clear(0xFFFFFFFF);

					// Update render texture
					SDL_UpdateTexture(renderTexture, &screenRect, pixelBuffer.getBuffer(), world.getValue<int>("window_width") * 4);

					// Render texture to screen
					SDL_RenderCopy(renderer, renderTexture, &screenRect, &screenRect);

					// Flip screen buffer
					SDL_RenderPresent(renderer);
				}
				// Toggle movement when player presses M
				else if (event.key.keysym.sym == SDLK_m)
				{
					collisionSystem->setEnabled(!collisionSystem->isEnabled());
					movementSystem->setEnabled(!movementSystem->isEnabled());
					gravitySystem->setEnabled(!gravitySystem->isEnabled());
				}
				// Add 10 balls when user presses right arrow
				else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
				{
					world.getManager<BallManager>()->createBalls(100);
				}
				// Remove 10 balls when user presses left arrow
				else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
				{
					world.getManager<BallManager>()->destroyBalls(100);
				}
			}
		}

		// Update game
		world.onLoopStart();
		world.setDelta(dt*GAME_SPEED);

        world.updateSystems();

		// Run update systems
		collisionSystem->update();
		movementSystem->update();
		gravitySystem->update();

		if (world.getValue<bool>("rendering_enabled"))
		{
			// Clear window
			SDL_RenderClear(renderer);

			// Clear buffer
            pixelBuffer.clear(CLEAR_COLOUR);

			// Run rendering system
			renderingSystem->update();

			// Blit buffer to screen renderer
			SDL_UpdateTexture(renderTexture, &screenRect, pixelBuffer.getBuffer(), world.getValue<int>("window_width") * 4);

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
			const char* renderingEnabled = (renderingSystem->isEnabled() ? "Enabled" : "Disabled");
			const char* movementEnabled = (movementSystem->isEnabled() ? "Enabled" : "Disabled");
			snprintf(titleBuffer, titleBufferLen, TITLE_FORMAT, ballManager->ballCount(), fps, renderingEnabled, movementEnabled);

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
