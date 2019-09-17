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
		
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); 
		if (!renderer) { destroy(); throw std::runtime_error("SDLRender::init : could not create a renderer..."); } 

		// don't init anymore
		has_init = true;	
}


void rle::render::SDLRender::update() {
        SDL_Texture* image_sum = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tile_x, tile_y);
}

void rle::render::SDLRender::push(unsigned int x, unsigned int y, std::string texture_path){
        
        std::vector<SDL_Texture*>* textures = at(x, y);
        
        if(textures == nullptr){
                textures = new std::vector<SDL_Texture*>(); 
        }

        textures->push_back(texture_table[texture_path]);
        
}        

void rle::render::SDLRender::destroy(){
	SDL_FreeSurface(surface); 
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	// delete texture_table
	surface = nullptr;
	window = nullptr; 
	renderer = nullptr;
	has_init = false; 
}

void rle::render::SDLRender::clear(){
        std::fill(texture_buf.begin(), texture_buf.end(), nullptr); 
}

rle::render::SDLRender::~SDLRender(){
	destroy();
}

void rle::render::SDLRender::pushTextureToTable(std::string path){
        SDL_Texture* tex = IMG_LoadTexture(renderer, path.c_str());
        texture_table.insert(std::pair<std::string, SDL_Texture*>(path, tex));            
}
