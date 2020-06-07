#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <map>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}
#include <LuaBridge.h>

namespace rle{
	namespace system{
		class System;
	}
	namespace tile{
		class TileMap;
	}
	namespace entity{
		class Entity;
	}
	struct RLE{	
		lua_State* L;
		
		std::vector<std::string> global_component_table; // keeps list of components loaded into main state
		std::map<std::string, system::System*> global_system_table; // stores pointers to all systems with their calls which have already been loaded into the lua state
		std::map<std::string, entity::Entity*> global_entity_table; // keeps a list of all entities
		std::map<std::string, tile::TileMap*> tile_map_table; // keeps a list of all tilemaps
		
		bool running = false;
		
		std::string current_entity_context = "undefined"; // keeps track of the current entity gamecontroller is using 

		RLE();
		virtual ~RLE();

		lua_State* LuaState() { return L; }
		
		void NewEntity(entity::Entity& entity);
		void NewEntity(entity::Entity* entity);
		void NewEntity(std::string name, std::string tilemap, std::vector<std::string> components, unsigned int x, unsigned int y, unsigned int z);
		void DelEntity(unsigned int index);
		void DelEntity(std::string name);

		void NewTileMap(tile::TileMap tilemap);
		void DelTileMap(std::string name);

		void AddSystem(system::System* system);  
		void DelSystem(std::string system);
		
		void AddComponent(std::string component);
		
		bool CheckSystem(std::string system);
		bool CheckComponent(std::string component);
		bool CheckEntity(std::string entity);
		bool CheckTileMap(std::string tilemap); 
		
		system::System& GetSystem(std::string system);
		entity::Entity& GetEntity(std::string entity);
		tile::TileMap& GetTileMap(std::string tilemap);

		std::vector<std::string>& GetComponentTable() { return global_component_table; }
		std::map<std::string, system::System* > & GetSystemTable() { return global_system_table; }
		std::map<std::string, entity::Entity* > & GetEntityTable() { return global_entity_table; }

		std::string GetContext() { return current_entity_context; }
		void SetContext(std::string context) { current_entity_context = context; }

	protected:
		void DelComponent(unsigned int index);
		void DelComponent(std::string component);

		void InitLua();
		void InitEntities();
	};
}
