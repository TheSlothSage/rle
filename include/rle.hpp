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
	class RLE{	
		lua_State* L;
		std::vector<std::string> global_component_table; // keeps list of components loaded into main state
		std::vector<system::System*> global_system_table; // stores pointers to all systems with their calls which have already been loaded into the lua state
		std::vector<entity::Entity*> global_entity_table; // keeps a list of all entities
		std::vector<tile::TileMap*> tile_map_table; // keeps a list of all tilemaps

		bool running = false;
	public:
		RLE();
		virtual ~RLE();
		void Start();
		bool State();

		lua_State* LuaState() { return L; }
		
		void NewEntity(entity::Entity& entity);
		void DelEntity(unsigned int index);
		void DelEntity(std::string name);

		void NewTileMap(tile::TileMap& tilemap);
		void DelTileMap(unsigned int index);
		void DelTileMap(std::string name);

		void AddSystem(system::System* system);  
		void DelSystem(unsigned int index);
		void DelSystem(std::string system);
		
		void AddComponent(std::string component);
		
		bool CheckSystem(std::string system);
		bool CheckComponent(std::string component);
		bool CheckEntity(std::string entity);
		bool CheckTileMap(std::string tilemap); 
		
		system::System& GetSystem(std::string system);
		entity::Entity& GetEntity(std::string entity);
		tile::TileMap& GetTileMap(std::string tilemap);

	protected:
		void DelComponent(unsigned int index);
		void DelComponent(std::string component);

		void InitLua();
		void InitEntities();
	};
}
