// main.cpp
#include <iostream>
#include "gamecontroller.hpp"


int main() {
	try{
		//rle::RLE& rle = *(new rle::RLE());
		rle::interface::basic_irle iface;
		iface.Do().GiveInstance().Start();
	} catch(std::exception& e){
		std::cerr << e.what() << std::endl;	
		return 0; // EXIT_FAIL
	}
	return 1;
}

 
