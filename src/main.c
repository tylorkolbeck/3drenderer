#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <stdbool.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_AppResult is_running = 2;

SDL_AppResult initialize_window(void) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
		fprintf(stderr, "error initializing SDL. \n" );
		SDL_Log("could not initialize SDL %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	// Create a SDL window
	window = SDL_CreateWindow("3D Renderer", 600, 600, SDL_WINDOW_BORDERLESS);
	if (!window) {
		SDL_Log("could not create SDL window %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	// Create an SDL renderer
	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		SDL_Log("could not create SDL renderer %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	
	return SDL_APP_CONTINUE;
}

int main(void) {
	SDL_AppResult is_running = initialize_window();
	printf("%i", is_running);
	return 0;
}
