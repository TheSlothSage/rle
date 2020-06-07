#pragma once

#include <string>
#include <vector>
#include "component.hpp"
#include "system.hpp"
#include "tilemap.hpp"

void recursive_ResolveDependencies(std::string component, lua_State* L, std::vector<rle::component::Component*>* vec);  

namespace rle{	
	namespace entity{		
		class Entity{
			std::string name;
			tile::Tile& tile;
			tile::TileMap& tilemap;
			lua_State* L; 
			unsigned int x;
			unsigned int y;
			unsigned int z;

		public:
			bool callInit = true;

			Entity(lua_State* L, tile::TileMap& tilemap, std::vector<std::string> components, std::string name, unsigned int x, unsigned int y, unsigned int z);
			~Entity();
			
			std::vector<component::Component*> component_table;

			std::string Name() { return name; }
			tile::Tile& Tile() { return tile; }
			std::vector<component::Component*>& Components() { return component_table; }

			component::Component& GetComponent(std::string name);
			void AddComponent(rle::component::Component* component) {                                 
				Components().push_back(component);
			}
			unsigned int X() { return x; }
			unsigned int Y() { return y; }
			unsigned int Z() { return z; }

			void setX(int x_) { x = x_; UpdateTilePtr();}
			void setY(int y_) { y = y_; UpdateTilePtr();}
			void setZ(int z_) { z = z_; UpdateTilePtr();}
			
			// these two are the same, but since i don't have an editor with 
			// fast reference replacement im just gonna leave this here. 
			// both are inlined anyways so who cares.TileMap
			
			
			tile::TileMap& getTileMap() { return tilemap; } 
			
			void setTileMap(tile::TileMap& tilemap_) { 
			        
			        tilemap = tilemap_;
			        
			        x = 0;
			        y = 0;
			        z = 0;
			        
			        UpdateTilePtr(); 
			}
			
			// interface level must deal with this because passing
			// rle to everything is dumb and it's not how i set up 
			// all the code unfortunately 
			// --- update ---- 
			// strike that, interface doesn;t have to do shit
			
			void UpdateTilePtr();			
		};
	}
}
