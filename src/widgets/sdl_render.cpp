#include "sdl_render.hpp"
#include "lua_register.hpp"

rle::game::widgets::SDLRender::SDLRender() : Controller_Widget(Widget_State<SDLRender>()) {}


void initSDL(){

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
