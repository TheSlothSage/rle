#include "SDL.h"
#include "renderer.hpp"

namespace rle{
	namespace render{

		struct SDLRender : rle::render::Element<SDLRender>{
			
			SDLRender(unsigned int x, unsigned int y) : rle::render::Element<SDLRender>(x, y) {}
			~SDLRender();

			void init();
			void update(std::vector<rle::entity::Entity*>& ent);	
			void destroy(); 

			bool has_init = false;

			SDL_Window* window = nullptr;
			SDL_Surface* surface = nullptr;
	
		};
	}
}


