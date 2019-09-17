#include "SDL.h"
#include "SDL_image.h"
#include "renderer.hpp"

#include <algorithm>
#include <map>

namespace rle{
	namespace render{

		class SDLRender : public rle::render::Element<SDLRender>{
				
			std::vector<std::vector<SDL_Texture*>*> texture_buf; 	
			
			// Serves as a lookup table for textures. Updated by 
			// the texture component 
			std::map<std::string, SDL_Texture*> texture_table;
			
			unsigned int tile_x = 32; // x in pixels
                        unsigned int tile_y = 32; // y in pixels
                        
                        unsigned int max_tile_x;
                        unsigned int max_tile_y;
                        
                        
                        bool has_init = false;

                        SDL_Window* window = nullptr;
                        SDL_Surface* surface = nullptr;
                        SDL_Renderer* renderer = nullptr;
                        
                        
                        std::vector<SDL_Texture*>* at(unsigned int x, unsigned int y){
                                return texture_buf.at(x + x*y);
                        } 
                        
                    public:    
                    
                        SDLRender(unsigned int x_, unsigned int y_) 
                                : max_tile_x(x_/tile_x), max_tile_y(y_/tile_y), rle::render::Element<SDLRender>(x_, y_)
                                                { 
                                                        texture_buf.reserve(max_tile_x * max_tile_y); 
                                                        std::fill(texture_buf.begin(), texture_buf.end(), nullptr);
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
                        
			void init();
			void update();	
			void destroy(); 
			void clear();
			
			// will be called by lua scripts to push entities to the renderer
			void push(unsigned int x, unsigned int y, std::string texture_path); 
		};
	}
}


