#include "sdl_render.hpp"
#include "lua_register.hpp"

rle::game::widgets::SDLRender::SDLRender() : Controller_Widget(Widget_State<SDLRender>()) {}

struct GlobalSDLRender {
	static rle::render::SDLRender sdl;
	static unsigned int x;
	static unsigned int y;
}; 

unsigned int GlobalSDLRender::x = 800;
unsigned int GlobalSDLRender::y = 600; 

rle::render::SDLRender GlobalSDLRender::sdl = rle::render::SDLRender(GlobalSDLRender::x, GlobalSDLRender::y); 

void initSDL(unsigned int x, unsigned int y){
	GlobalSDLRender::sdl.setX(x);
	GlobalSDLRender::sdl.setY(y);

	GlobalSDLRender::sdl.init(); 
}

void rle::game::widgets::SDLRender::Do() {
	
	if(register_functions){	
		luabridge::getGlobalNamespace(rle::game::Irle::irle->Do().LuaState())
			.beginNamespace("rle")
				.beginNamespace("sdl") 
					.addFunction("init", initSDL)
			     	.endNamespace()  			
			.endNamespace(); 
		
		register_functions = false;
	}

}
