#include "entity.hpp"

rle::entity::Entity::Entity(lua_State* _L, tile::TileMap& _tilemap, std::vector<std::string> components,
			    std::string _name, unsigned int _x, unsigned int _y, unsigned int _z) :
	                         L(_L), name(_name), x(_x), y(_y), z(_z), tilemap(_tilemap), tile(_tilemap(_x,_y,_z)) {
	
				for(std::vector<std::string>::iterator iter = components.begin(); iter != components.end();
				    ++iter)
				{
					component_table.push_back(new component::Component(*iter, L));			
				}
}

rle::entity::Entity::~Entity(){
	for(component::Component*& c : component_table){
		delete c;
	}
}

void rle::entity::Entity::UpdateTilePtr(){
	for(std::vector<rle::entity::Entity*>::iterator iter = tile.entities.begin(); iter != tile.entities.end(); ++iter){
		if((*iter)->Name() == name){
			tile.entities.erase(iter);
			break;
		}
	}
	tile = tilemap(x, y, z);
	tile.entities.push_back(this); 
}
