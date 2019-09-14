// main.cpp
#include <iostream>
#include "rle_engine.hpp"


int main() {
	try{

		auto cw = rle::game::widgets::DebugCall();
		auto dc = rle::game::widgets::CriticalWidget();
		auto sdl = rle::game::widgets::SDLRender(); 

		auto g = rle::game::GameController();
		g.LoadWidget(cw);
		g.LoadWidget(dc);
		g.LoadWidget(sdl); 
		g.Start();
	} catch(std::exception& e){
		std::cerr << e.what() << std::endl;	
		return 0; // EXIT_FAIL
	}
	return 1;
}

 
