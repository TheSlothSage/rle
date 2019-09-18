#include "sdl_render.hpp"
#include "lua_register.hpp"

rle::game::widgets::SDLRender::SDLRender() : Controller_Widget(Widget_State<SDLRender>()) {}

static rle::render::SDLRender* sdl = nullptr;



void initSDL(unsigned int x, unsigned int y, std::string path) {

	sdl = new rle::render::SDLRender(x, y, path);
	
} 

void pushTexture(unsigned int x, unsigned int y, std::string tex_path){
        sdl->push(x, y, tex_path); 
}


void clear() { sdl->clear(); } 

void pushTextureToTable(std::string path){
	sdl->pushTextureToTable(path); 
}

unsigned int getMax_X() { return sdl->getTile_X(); }
unsigned int getMax_Y() { return sdl->getTile_Y(); }

void setMax_X(unsigned int x) { sdl->setTile_X(x); }
void setMax_Y(unsigned int y) { sdl->setTile_Y(y); }

void update() {
        sdl->update(); 
}

void rle::game::widgets::SDLRender::Do() {
	
	if(register_functions){	
		luabridge::getGlobalNamespace(rle::game::Irle::irle->Do().LuaState())
			.beginNamespace("rle")
				.addFunction("sdl_init", &initSDL)
				.addFunction("sdl_clear", &clear)
				.addFunction("sdl_update", &update)
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
