// main.cpp
#include <iostream>
#include "rle.hpp"

void printmessage(const std::string& s) { std::cout << s << std::endl; }

int main() {
	try{
	rle::RLE* rle = new rle::RLE();
	} catch(std::exception& e){
		std::cerr << e.what() << std::endl;
		return 0; // EXIT_FAIL
	}
	return 1;
}
