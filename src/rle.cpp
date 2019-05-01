#include "rle.hpp"
#include "entity.hpp"

rle::RLE::RLE(){
	//init test tilemap, this should be deleted and should use the initstate script
	tile_map_table.push_back(new tile::TileMap("test", 100, 100, 100));
	InitLua();	
}

void rle::RLE::InitLua(){
	//create lua state and open all the libraries
	L = luaL_newstate();
	luaL_openlibs(L);
	// create the component and system tables
	luaL_dostring(L, "components = {}");
	luaL_dostring(L, "systems = {}");
}

void rle::RLE::InitEntities(){
	std::vector<std::string> components;
	std::string name;
	unsigned int x;
	unsigned int y;
	unsigned int z;

	if (luaL_loadfile(L, "rle.initstate.lua") ||
	    lua_pcall(L, 0, 1, 0)) {
		std::cout << "rle::RLE::RLE() : rle.initsate.lua either does not exist or has been moved" << std::endl;
		exit;
	}
	
	luabridge::LuaRef init_entity_table = luabridge::LuaRef::fromStack(L, -1);

	if(init_entity_table.isNil()){
		std::cout << "rle::RLE::RLE() : entities table does not exist in rle.initstate.lua" << std::endl;
		exit;
	}

	
	std::cout << "Initializing Init Entities..." << std::endl;
	
	for(int i = 1; i <= lua_rawlen(L, -1); ++i){
		luabridge::LuaRef iter_table = init_entity_table[i];
		
		if(iter_table.isNil()){
			std::cout << "rle::RLE::RLE() : failed" << std::endl;
			exit;
		}
		// probably should check if these exist			
		name = iter_table["name"].cast<std::string>();	     
		x = iter_table["x"].cast<unsigned int>();
		y = iter_table["y"].cast<unsigned int>();
		z = iter_table["z"].cast<unsigned int>();
		luabridge::LuaRef component_table = iter_table["components"];
		if(component_table.isNil()){
			throw "RLE::Component::Component() : Entity does not have asociated component table"; 
		}
			
		std::cout << "\t-Entity: " << name << std::endl;
		std::cout << "\t\t-At(" << x << "," << y << "," << z << ")" << std::endl;
		std::cout << "\t\t-Components" << std::endl;	       

		component_table.push();	
		lua_pushnil(L); 
		while(lua_next(L, -2)){
			if(lua_isstring(L, -1)){
				std::cout << "\t\t\t-" << std::string(lua_tostring(L, -1)) << std::endl;
				components.push_back(lua_tostring(L, -1));
			}
			else{
				throw std::runtime_error("rle::RLE::RLE() : enitities->component table contains value other than string in rle.initstate.lua");
			}
			std::cout << "\t\tSpawning Entity..." << std::endl;
			global_entity_table.push_back(new entity::Entity(L, *(tile_map_table.at(0)), components, name, x, y, z));	   
			components.clear();
			// pop value
			lua_pop(L, 1); 
		}
		// clear stack
		lua_settop(L,0);
	}

	// Now loop through everything and populate all the global tables

	std::cout << "Loading Initialized Entities Into Tables..." << std::endl;
	
	for(entity::Entity*& ent : global_entity_table){
		std::vector<component::Component*>& components = ent->Components();
		for(component::Component*& com : components){
			if(!CheckComponent(com->Name())){
				global_component_table.push_back(com->Name());
			}
			std::vector<system::System*>& systems = com->Systems();
			for(system::System*& sys : systems){
				if(!CheckSystem(sys->Name())){
					global_system_table.push_back(sys); 
				}
			}			
		}
	}
	std::cout << "Initialization of RLE state complete!" << std::endl;
}

void rle::RLE::Start(){
	// Init Tile Maps 
	InitEntities();
	running = true;
	// Begin Game Loop
}

bool rle::RLE::State(){
	return running; 
}

void rle::RLE::NewEntity(entity::Entity& entity){
	global_entity_table.push_back(new entity::Entity(entity)); 
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
