// main.cpp
#include <iostream>
#include "rle_engine.hpp"


int main() {
	try{
		//rle::RLE& rle = *(new rle::RLE());
		auto g = rle::game::GameController();
		g.LoadWidget(rle::game::widgets::DebugCall());
		g.LoadWidget(rle::game::widgets::CriticalWidget());
		g.Start();
	} catch(std::exception& e){
		std::cerr << e.what() << std::endl;	
		return 0; // EXIT_FAIL
	}
	return 1;
}

 
