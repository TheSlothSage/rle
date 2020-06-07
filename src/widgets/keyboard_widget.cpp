#include "keyboard_widget.hpp"
#include "SDL.h"
rle::game::widgets::KeyboardWidget::KeyboardWidget() : Controller_Widget(Widget_State<KeyboardWidget>()) {}

// hhh this is going to be big but idc anymore
std::string getKeyDown() {
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type && event.type == SDL_KEYDOWN) {
	return SDL_GetKeyName(event.key.keysym.sym);
    }
    return "";
}

void rle::game::widgets::KeyboardWidget::Do() {
	if (register_functions) {
			
		lua_State* L = irle->Do().LuaState();
		luabridge::getGlobalNamespace(L)
		    .beginNamespace("rle")

			.addFunction("getKeyDown", &getKeyDown)

		    .endNamespace();
		register_functions = false;
	}
	
}