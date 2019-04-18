#include "tilemap.hpp"
#include <iostream>

rle::tile::TileMap::TileMap(std::string _name, unsigned int x, unsigned int y, unsigned int z){
	name = _name;
	size = x*y*z;
	max_x = x;
	max_y = y;
	max_z = z;
	tiles.reserve(size);
	for(unsigned int z = 0; z < max_z; ++z){
		for(unsigned int y = 0; y < max_y; ++y){
			for(unsigned int x = 0; x < max_x; ++x){				
				tiles.push_back(Tile(x, y, z)); 
			}
		}
	}
}

rle::tile::TileMap::~TileMap(){
	for(tile::Layer*& layer : data_layers){
		delete(layer);
	}
}

rle::tile::Tile& rle::tile::TileMap::operator[](unsigned int i){
	return tiles.at(i);
}

rle::tile::Tile& rle::tile::TileMap::operator()(unsigned int x, unsigned int y, unsigned int z){
	return tiles.at(x + y*max_x + z*max_y*max_x); 
}

template<typename T>
void rle::tile::TileMap::NewLayer(std::string name, T init_val){
	data_layers.push_back(new rle::tile::Derived_Layer<T>(name, init_val, size)); 
}

rle::tile::Layer& rle::tile::TileMap::GetLayer(std::string _name){
       	for(std::vector<rle::tile::Layer*>::iterator i = data_layers.begin(); i != data_layers.end(); ++i){
		if((*i)->Name() == name){			
			return **i;
		}
	}	
	throw std::invalid_argument ("rle::tile::TileMap::GetLayer(std::string) " + name + " does not exist"); 
}

rle::tile::Layer& rle::tile::TileMap::GetLayer(unsigned int index){
	if(index < data_layers.size()){
		return **(data_layers.begin() + index); 
	}
	else{
		throw std::out_of_range("rle::tile::TileMap::GetLayer(unsigned int) : Iterator position out of range");
	} // Throw an exception I guess 
}

void rle::tile::TileMap::DelLayer(std::string name){
	for(std::vector<rle::tile::Layer*>::iterator i = data_layers.begin(); i != data_layers.end(); ++i){
		if((*i)->Name() == name){
			delete(*i);
			data_layers.erase(i);
			return;
		}
	}	
	throw std::invalid_argument ("rle::tile::TileMap::DelLayer(std::string) :  " + name + " does not exist"); 
}
 
inline void rle::tile::TileMap::DelLayer(unsigned int index){
	if(index < data_layers.size()){
		data_layers.erase(data_layers.begin() + index); 
	}
	else{
		throw std::out_of_range("rle::tile::TileMap::DelLayer(unsigned int) : Iterator position out of range");
	} 
}

