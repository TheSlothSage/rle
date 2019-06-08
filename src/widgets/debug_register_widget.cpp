#include "debug_register_widget.hpp"

// functions that will be registered

void Debug_Log(std::string msg) {
	std::cout << msg << std::endl;
}

void Debug_Number(lua_Number num) {
	std::cout << num << std::endl;
}

void Error_Log(std::string msg) {
	std::cerr << msg << std::endl;
}

rle::game::widgets::DebugCall::DebugCall() : Controller_Widget(Widget_State<DebugCall>()){}

void rle::game::widgets::DebugCall::Do() {
	if (register_functions) {
		lua_State* L = irle->Do().LuaState();
		luabridge::getGlobalNamespace(L)
			.beginNamespace("rle")
				.beginNamespace("debug")
					.addFunction("Log", Debug_Log)
					.addFunction("Num", Debug_Number)
					.addFunction("Error", Error_Log)
				.endNamespace()
			.endNamespace();
		register_functions = false;
	}
}

