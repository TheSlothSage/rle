#include "SDLRender.hpp"

#include <SDL.h>


void rle::render::SDLRender::init() {
	
	// init sdl	
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
       		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}	

	        window = SDL_CreateWindow
			("game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, getX(), getY(), SDL_WINDOW_SHOWN );
		if(!window) { throw std::runtime_error("SDLRender::init : could not create window..."); }
		
		surface = SDL_GetWindowSurface(window);
		if (!surface) { destroy(); throw std::runtime_error("SDLRender::init : could not get surface of window..."); } 	
		
		// don't init anymore
		has_init = true;	
}

// Update takes a list of entities, checks if its size is equal to the available rendering space, and draws them to the screen.

void rle::render::SDLRender::update(std::vector<rle::entity::Entity*>& ent) {
		
}

void rle::render::SDLRender::destroy(){
	SDL_FreeSurface(surface); 
	SDL_DestroyWindow(window);
	surface = nullptr;
	window = nullptr; 
	has_init = false; 
}

rle::render::SDLRender::~SDLRender(){
	destroy();
}


