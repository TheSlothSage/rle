#include "entity.hpp"
#include <iostream>

void recursive_ResolveDependencies(std::string name, lua_State* L, std::vector<rle::component::Component*>* vec){
        
	/*
         * The following code will spawn component dependencies. It'll need to be documented
         * that I still haven't added in support for shared dependencies. Keeping 
         * dependency trees shallow is the goal here. Having a mess of components goes against
         * the spirit of RLE. 
         *      - The Sloth Sage
        */

        luabridge::LuaRef component_table = luabridge::LuaRef::getGlobal(L, "components");
        luabridge::LuaRef component = component_table[name];
        luabridge::LuaRef dependencies = component["dependencies"];
        
        if(!dependencies.isNil()) {
                
                std::string component_name;
                dependencies.push();
                lua_pushnil(L);
                
                while(lua_next(L, -2)) {
                        if(lua_isstring(L, -1)){
                                component_name = luabridge::LuaRef::fromStack(L, -1).cast<std::string>();
                        }
                        else{
                                throw std::runtime_error("rle::entity::recursive_ResolveDependencies : non string in dependencies for component " + name);
                        }
                        // oooo recursive algorithm aren't i clever
                        recursive_ResolveDependencies(component_name, L, vec);
                        vec->push_back(new rle::component::Component(component_name, L));
                        lua_pop(L, 1); 
                }
                lua_pop(L,1);
        } // end this whole lua nonsense        
}


// the give a shit levels just keep going down to try to get this mf to work

rle::entity::Entity::Entity(lua_State* L_, tile::TileMap& _tilemap, std::vector<std::string> components,
			    std::string _name, unsigned int _x, unsigned int _y, unsigned int _z) :
	                         L(L_), name(_name), x(_x), y(_y), z(_z), tilemap(_tilemap), tile(_tilemap(_x,_y,_z)) {
	
				for(std::vector<std::string>::iterator iter = components.begin(); iter != components.end();
				    ++iter)
				{
					std::cout << "does this even work\n";
					
					// resolve dependencies :-\
				        
				        recursive_ResolveDependencies(*iter, L, &component_table); 
				        
                                        
                                        //finially, we push our desired component
                                        
					component_table.push_back(new component::Component(*iter, L));
				}
				UpdateTilePtr();
}

rle::entity::Entity::~Entity(){
	for(component::Component*& com : component_table){ delete com; }
}

rle::component::Component& rle::entity::Entity::GetComponent(std::string name){
	
	for(unsigned int i = 0; i < component_table.size(); ++i){
		
		
		if((component_table[i])->Name() == name){
			return *component_table[i];
		}
	}
	throw std::runtime_error("Entity::GetComponent : No component with name : " + name); 
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

