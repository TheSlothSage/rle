#pragma once

#include <vector>
#include <string>
#include <iostream>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}
#include <LuaBridge.h>
#include "system.hpp"
#include "entity.hpp"
#include "tilemap.hpp"

namespace rle{
		
	class RLE{	
		lua_State* L; 
		std::vector<system::System*> global_system_table;
		std::vector<entity::Entity*> global_entity_table;
		std::vector<tile::TileMap*> tile_map_table; 
	public:
		RLE(); 
		void NewEntity(entity::Entity& entity);
		void DelEntity(unsigned int index);
		void DelEntity(std::string name);
		void NewTileMap(tile::TileMap& tilemap);
		void DelTileMap(unsigned int index);
		void DelTileMap(std::string name); 
	};
}
