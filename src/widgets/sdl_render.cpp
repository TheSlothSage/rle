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

void initSDL(unsigned int x, unsigned int y) {
	GlobalSDLRender::sdl.setX(x);
	GlobalSDLRender::sdl.setY(y);
	
        GlobalSDLRender::sdl.destroy();
	GlobalSDLRender::sdl.init(); 
} 

void pushTexture(unsigned int x, unsigned int y, std::string tex_path){
        GlobalSDLRender::sdl.push(x, y, tex_path); 
}

void draw(){
    
}

void clear() { GlobalSDLRender::sdl.clear(); } 

void pushTextureToTable(std::string path){
	GlobalSDLRender::sdl.pushTextureToTable(path); 
}

unsigned int getMax_X() { return GlobalSDLRender::sdl.getTile_X(); }
unsigned int getMax_Y() { return GlobalSDLRender::sdl.getTile_Y(); }

void setMax_X(unsigned int x) { GlobalSDLRender::sdl.setTile_X(x); }
void setMax_Y(unsigned int y) { GlobalSDLRender::sdl.setTile_Y(y); }

void rle::game::widgets::SDLRender::Do() {
	
	if(register_functions){	
		luabridge::getGlobalNamespace(rle::game::Irle::irle->Do().LuaState())
			.beginNamespace("rle")
				.addFunction("sdl_init", &initSDL)
				.addFunction("sdl_draw", &draw)
				.addFunction("sdl_clear", &clear)
				.addFunction("sdl_pushTexture", &pushTexture)
				.addFunction("sdl_pushTextureToTable", &pushTextureToTable)
			        .addFunction("sdl_getMax_X", &getMax_X)
			        .addFunction("sdl_setMax_X", &setMax_X)
			        .addFunction("sdl_getMax_Y", &getMax_Y)
			        .addFunction("sdl_setMax_Y", &setMax_Y)
			.endNamespace(); 
		
		register_functions = false;
	}
}
