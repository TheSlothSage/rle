#pragma once

#include <string>
#include <vector>
#include "component.hpp"
#include "system.hpp"
#include "tilemap.hpp"

namespace rle{	
	namespace entity{		
		class Entity{
			std::string name;
			std::vector<component::Component*> component_table;		       
			tile::Tile& tile;
			tile::TileMap& tilemap;
			lua_State* L; 
			unsigned int x;
			unsigned int y;
			unsigned int z;
		public:
			Entity(lua_State* _L, tile::TileMap& _tilemap, std::vector<std::string> components, std::string _name, unsigned int _x, unsigned int _y, unsigned int _z);
			~Entity();
			
			std::string Name() { return name; }
			tile::Tile& Tile() { return tile; }
			std::vector<component::Component*>& Components() { return component_table; }

			component::Component& GetComponent(std::string name);
			tile::TileMap& GetTileMap(std::string name);		       

			unsigned int X() { return x; }
			unsigned int Y() { return y; }
			unsigned int Z() { return z; }

			tile::TileMap& TileMap() { return tilemap; } // interface level must deal with this because passing rle to everything is dumb and it's not how i set up all the code unfortunately 
			
			void UpdateTilePtr();			
		};
	}
}
