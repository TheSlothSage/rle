#include "rle.hpp"
#include "entity.hpp"
#include <iterator>

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
	for(std::map<std::string, entity::Entity*>::iterator ent = global_entity_table.begin(); 
	        ent != global_entity_table.end(); 
	                ++ent) 
	{ 
	        delete ent->second; 
	} 

        
}

void rle::RLE::NewEntity(entity::Entity& entity){
	global_entity_table.insert(
	        std::pair<std::string, entity::Entity*> 
	                (entity.Name(), new entity::Entity(entity))
	); 
}

void rle::RLE::NewEntity(entity::Entity* entity) {
	global_entity_table.insert(
	        std::pair<std::string, entity::Entity*>
	                (entity->Name(), entity)
	);
}
void rle::RLE::NewEntity(
        std::string name, std::string tilemap, 
                std::vector<std::string> components, 
                        unsigned int x, unsigned int y, unsigned int z) 
        
{
        global_entity_table.insert
        (
                std::pair<std::string, entity::Entity*>
                (name, new entity::Entity(L, GetTileMap(tilemap), components, name, x, y, z))
	);

                
}

void rle::RLE::DelEntity(std::string name){
        global_entity_table.erase(global_entity_table.find(name));
}

void rle::RLE::AddComponent(std::string component){
	global_component_table.push_back(component); 
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
	global_system_table[system->Name()] = system; 
}

void rle::RLE::DelSystem(std::string system){
        global_system_table.erase(global_system_table.find(system));
}

rle::system::System& rle::RLE::GetSystem(std::string system){
	return *(global_system_table[system]); 
}


rle::entity::Entity& rle::RLE::GetEntity(std::string entity){
	return *(global_entity_table[entity]);
}

rle::tile::TileMap& rle::RLE::GetTileMap(std::string tilemap){
	return *(tile_map_table[tilemap]);
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
        return (global_system_table.find(name) == global_system_table.end());
}

bool rle::RLE::CheckEntity(std::string name){
	return (global_entity_table.find(name) == global_entity_table.end());
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
	tile_map_table[tilemap.Name()] = new tile::TileMap(tilemap);
}

void rle::RLE::DelTileMap(std::string name) {
	tile_map_table.erase(tile_map_table.find(name)); 
}
