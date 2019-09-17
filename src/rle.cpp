#include "rle.hpp"
#include "entity.hpp"

rle::RLE::RLE(){
	//init test tilemap, this should be deleted and should use the initstate script
	InitLua();	
}

void rle::RLE::InitLua(){
	//create lua state and open all the libraries
	L = luaL_newstate();
	luaL_openlibs(L);
	// create the component, system, and tilemap tables
	luaL_dostring(L, "components = {}");
	luaL_dostring(L, "systems = {}");
	luaL_dostring(L, "tilemaps = {}");
}

rle::RLE::~RLE(){
	for(entity::Entity*& ent : global_entity_table) { delete ent; } 
}

void rle::RLE::NewEntity(entity::Entity& entity){
	global_entity_table.push_back(new entity::Entity(entity)); 
}

void rle::RLE::NewEntity(entity::Entity* entity) {
	global_entity_table.push_back(entity);
}
void rle::RLE::NewEntity(std::string name, std::string tilemap, std::vector<std::string> components, unsigned int x, unsigned int y, unsigned int z) {
	global_entity_table.push_back(new entity::Entity(L, GetTileMap(tilemap), components, name, x, y, z));
}

void rle::RLE::DelEntity(unsigned int index){
	if(index >= global_entity_table.size()){
		throw std::runtime_error("rle::RLE::DelEntity : index out of range");
	}
	global_entity_table.erase(global_entity_table.begin() + index); 
}

void rle::RLE::DelEntity(std::string name){
	for(std::vector<rle::entity::Entity*>::iterator iter = global_entity_table.begin();
	    iter != global_entity_table.end();
	        ++iter )
	{
		if((*iter)->Name() == name){
			global_entity_table.erase(iter);
			return;
		}
	}
	throw std::runtime_error("rle::RLE::DelEntity : No such name " + name);       
}

void rle::RLE::AddComponent(std::string component){
	global_component_table.push_back(component); 
}

void rle::RLE::DelComponent(unsigned int index){
	if(index >= global_component_table.size()){
		throw std::runtime_error("rle::RLE::DelComponent : index out of range"); 
	}
	global_component_table.erase(global_component_table.begin() + index); 
}

void rle::RLE::DelComponent(std::string component){
	for(std::vector<std::string>::iterator i = global_component_table.begin(); i != global_component_table.end(); ++i){
		if(*i == component){
			global_component_table.erase(i);
			return;
		}
	}
	throw std::runtime_error("rle::RLE::DelEntity : No such name " + component);
}

void rle::RLE::AddSystem(rle::system::System* system){
	global_system_table.push_back(system); 
}

void rle::RLE::DelSystem(unsigned int index){
	if(index >= global_system_table.size()){
		throw std::runtime_error("rle::RLE::DelSystem : index out of range"); 
	}
	global_system_table.erase(global_system_table.begin() + index);
}

void rle::RLE::DelSystem(std::string system){
	for(std::vector<rle::system::System*>::iterator i = global_system_table.begin(); i != global_system_table.end(); ++i){
		if((*i)->Name() == system){
			global_system_table.erase(i);
			return;
		}
	}
	throw std::runtime_error("rle::RLE::DelSystem : No such name " + system);

}

rle::system::System& rle::RLE::GetSystem(std::string system){
	for(system::System*& sys : global_system_table){
		if(sys->Name() == system){
			return *sys;
		}
	}
	throw std::runtime_error("rle::RLE::GetSystem : No such name " + system); 
}


rle::entity::Entity& rle::RLE::GetEntity(std::string entity){
	for(entity::Entity*& ent : global_entity_table){
		if(ent->Name() == entity){
			return *ent;
		}
	}
	throw std::runtime_error("rle::RLE::GetEntity : No such name " + entity); 
}

rle::tile::TileMap& rle::RLE::GetTileMap(std::string tilemap){
	for(tile::TileMap*& til : tile_map_table){
		if(til->Name() == tilemap){
			return *til;
		}
	}
	throw std::runtime_error("rle::RLE::GetTileMap : No such name " + tilemap);
}

bool rle::RLE::CheckComponent(std::string name){
	for(std::string& str : global_component_table){
		if(name == str){
			return true;
		}
	}
	return false;
}

bool rle::RLE::CheckSystem(std::string name){
	for(rle::system::System*& sys : global_system_table){
		if(sys->Name() == name){
			return true;
		}
	}
	return false;
}

bool rle::RLE::CheckEntity(std::string name){
	for(rle::entity::Entity*& ent : global_entity_table){
		if(ent->Name() == name) {
			return true;
		}
	}
	return false;
}

// Define this here: component populate function to use forward declaration of class RLE

void rle::component::Component::PopulateIntoRLEComponentTable(rle::RLE& rle){
	if(rle.CheckComponent(name)) { return; }
	else{
		rle.AddComponent(name); 
	}
}

// Define this here: system populate function to use forward declaration of class RLE

void rle::system::System::PopulateIntoRLESystemTable(rle::RLE& rle){
	if(rle.CheckSystem(name)) { return; }
	else{
		rle.AddSystem(this);
	}
}

// tilemap operations

void rle::RLE::NewTileMap(tile::TileMap tilemap)
{
	tile_map_table.push_back(new tile::TileMap(tilemap));
}

void rle::RLE::DelTileMap(std::string name) {
	for (std::vector<tile::TileMap*>::iterator it = tile_map_table.begin(); it != tile_map_table.end(); ++it) {
		if ((*it)->Name() == name) {
			tile_map_table.erase(it);
		}
	}
	throw std::runtime_error("Failed to find tilemap with name: " + name);
}

void rle::RLE::DelTileMap(unsigned int index) {
	try {
		tile_map_table.erase(tile_map_table.begin() + index);
	}
	catch (std::out_of_range & e) {
		throw std::runtime_error("Failed to find tilemap at index, as it was out of range");
	}
}
