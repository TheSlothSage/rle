#include "rle.hpp"

rle::RLE::RLE(){
	//init test tilemap, this should be deleted and should use the initstate script
	tile_map_table.push_back(new tile::TileMap("test", 100, 100, 100));

	L = luaL_newstate();

	// create the component and system tables
	luaL_dostring(L, "components = {}");
	luaL_dostring(L, "systems = {}");

	luaL_openlibs(L);
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
	// Actually get all the components and stuff
	//Push all this stuff out of the for loop
	
	std::vector<std::string> components;
	std::string name;
	unsigned int x;
	unsigned int y;
	unsigned int z;
	
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
		component_table.push();
		if(component_table.isNil())
		{
			std::cout << "rle::RLE::RLE() : components does not exist" << std::endl;
			exit;
		}
		else{
			lua_pushnil(L); 
			while(lua_next(L, -2)){
				if(lua_isstring(L, -1)){
					components.push_back(lua_tostring(L, -1));
				}
				else{
					throw std::runtime_error("rle::RLE::RLE() : enitities->component table contains value other than string in rle.initstate.lua");
				}
				global_entity_table.push_back(new entity::Entity(L, *(tile_map_table.at(0)), components, name, x, y, z));	   
				components.clear();
				// pop value
				lua_pop(L, 1); 
			}
			// pop key
			lua_pop(L, 1); 
		}
		// pop component_table
		lua_pop(L, 1);
	}
	// pop enitites global
	lua_pop(L, 1);
}

void rle::RLE::NewEntity(entity::Entity& entity){
	global_entity_table.push_back(new entity::Entity(entity)); 
}

void rle::RLE::DelEntity(unsigned int index){
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
	std::cout << "rle::RLE::DelEntity : No such name " << name << std::endl;
	exit;
}
