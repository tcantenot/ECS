#ifndef INITIALISATION_HPP
#define INITIALISATION_HPP

#include <SDL2/SDL.h>

SDL_Window* createWindow(const char* title, int width, int height);
SDL_Renderer* createRenderer(SDL_Window* window);
SDL_Texture* createTexture(SDL_Renderer* renderer, int width, int height);

#endif //INITIALISATION_HPP

