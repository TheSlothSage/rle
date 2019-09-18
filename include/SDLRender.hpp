#include "SDL.h"
#include "SDL_image.h"
#include "renderer.hpp"

#include <algorithm>
#include <map>

namespace rle{
	namespace render{

		/*
		 * I seemed to have designed my way into a corner, so this will act as a single instance renderer. I have attempted to avoid 
		 * things like this (in the hope that if one wanted to spawn multiple instances of rle in the same program it could be done),
		 * yet alas I could not for this. However, this is only used in the sdl_renderer widget, so there is still hope. This mostly 
		 * came out of a desperate need to get pixels on the screen, finally, and start developing other things. 
		 *
		 * This is nicely serving as the renderer state obj. Methods are exposed to lua for ease of use. Draw updates should be called
		 * in the sdl_renderer Do() virtual function. 	
		*/
		class SDLRender : public rle::render::Element<SDLRender>{
				
			std::vector<std::string> texture_buf; 	
			
			// Serves as a lookup table for textures. Updated by 
			// the texture component in the form of independent calls. 
			std::map<std::string, SDL_Texture*> texture_table;
			
			// pushTextureToTable is called before init is.
			// this just queues up textures to be loaded
			std::vector<std::string> path_queue; 
			
			unsigned int tile_x = 32; // x in pixels
                        unsigned int tile_y = 32; // y in pixels
                        
                        unsigned int max_tile_x;
                        unsigned int max_tile_y;
                        
                        
                        bool has_init = false;

                        SDL_Window* window = nullptr;
                        SDL_Surface* surface = nullptr;
                        SDL_Renderer* renderer = nullptr;
                        
                        std::string default_texture_path;
                        
                        std::string& at(unsigned int x_, unsigned int y_){
                                return texture_buf[x_  + max_tile_x*y_];
                        } 
                        
                    public:    
                    
                        SDLRender(unsigned int x_, unsigned int y_, std::string path) 
                                : max_tile_x(x_/tile_x), max_tile_y(y_/tile_y), rle::render::Element<SDLRender>(x_, y_)
                                        {
                                                texture_buf.resize(max_tile_x * max_tile_y); 
                                                std::fill(texture_buf.begin(), texture_buf.end(), "");
                                                init(path);
                                        }
                        
                        ~SDLRender();
                        
                        void setTile_X(unsigned int x){ 
                                tile_x = x;
                                max_tile_x = x/tile_x; 
                                texture_buf.resize(max_tile_x*max_tile_y);
				clear(); 
                        }
                        void setTile_Y(unsigned int y){
                                tile_y = y;
                                max_tile_y = y/tile_y;
                                texture_buf.resize(max_tile_x*max_tile_y);
				clear();
                        }
                        void setTile_XY(unsigned int x, unsigned int y){
                                tile_x = x;
                                max_tile_x = x/tile_x; 
                                tile_y = y;
                                max_tile_y = y/tile_y; 
                                texture_buf.resize(max_tile_x*max_tile_y); 
                                clear();

                        }
                        
                        void pushTextureToTable(std::string path);
                        
                        unsigned int getTile_X() const { return max_tile_x; }
                        unsigned int getTile_Y() const { return max_tile_y; }
                        
			void init(std::string path);
			void update();	
			void destroy(); 
			void clear();
			
			// will be called by lua scripts to push entities to the renderer
			void push(unsigned int x, unsigned int y, std::string texture_path); 
		};
	}
}


