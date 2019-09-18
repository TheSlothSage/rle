#include "SDLRender.hpp"

#include <SDL.h>

// now requires default texgture path
void rle::render::SDLRender::init(std::string default_texture_path_) {
	
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
               SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        }

	if(!has_init){
	        
	        // init sdl        
	        if(!window){
                        window = SDL_CreateWindow
                                ("game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, getX(), getY(), SDL_WINDOW_SHOWN );
                }
                if(!window) { throw std::runtime_error("SDLRender::init : could not create window..."); }
                
                if(!surface)  { surface = SDL_GetWindowSurface(window); }
                if (!surface) { throw std::runtime_error("SDLRender::init : could not get surface of window..."); }         
                
                if(!renderer){
                        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);                 

                }
                if (!renderer) { throw std::runtime_error("SDLRender::init : could not create a renderer..."); } 

                // don't init anymore
                has_init = true;        
                default_texture_path = default_texture_path_;
	        pushTextureToTable(default_texture_path);
	}
                
}


void rle::render::SDLRender::update() {
	
	// will need this later
	//SDL_Texture* image_sum = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tile_x, tile_y);
        
        SDL_Rect texture_rect;
        
        texture_rect.x = 0;
        texture_rect.y = 0;
        texture_rect.w = tile_x;
        texture_rect.h = tile_y;

	for(unsigned x_ = 0; x_ < max_tile_x; ++x_) {
	
		texture_rect.x = tile_x * x_;
		
		for(unsigned y_ = 0; y_ < max_tile_y; ++y_){

                        texture_rect.y = tile_y * y_;
                        auto tex = at(x_, y_);
                        if(tex == ""){
                                SDL_RenderCopy(renderer, texture_table[default_texture_path], NULL, &texture_rect);  
                        }
                        else{
                                SDL_RenderCopy(renderer, texture_table[tex], NULL, &texture_rect);
                        }
		} // y	
	} // x
	
	SDL_RenderPresent(renderer); 
}

void rle::render::SDLRender::push(unsigned int x_, unsigned int y_, std::string texture_path){

        texture_buf[x_ + max_tile_x * y_] = texture_path; 
        
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
}

rle::render::SDLRender::~SDLRender(){
	destroy();
}

void rle::render::SDLRender::pushTextureToTable(std::string path){
        
        SDL_Texture* tex = IMG_LoadTexture(renderer, path.c_str());
        
        if(!tex){
                throw std::runtime_error("SDLRender::pushTextureToTable : no such image at location " + path);
        }
         
        texture_table.insert(std::pair<std::string, SDL_Texture*>(path, tex));
}
