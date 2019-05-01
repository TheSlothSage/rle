 #pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge.h"

#include <string>
#include <vector>
#include <map>
#include "system.hpp"

namespace rle{
	namespace component{
		
		class Component{
			std::string name;
			std::map<std::string, luabridge::LuaRef> data;			 
			std::vector<system::System*> system_table;
			lua_State * L;
						
			void GetSystems();
			void GetData(); 
		public:
			Component(std::string _name, lua_State* _L);
			~Component();
			std::string Name(){ return name; }				       	
			luabridge::LuaRef& Get(std::string data);
			std::vector<system::System*>& Systems();
			void PopulateIntoRLEComponentTable(rle::RLE& rle); // in rle.cpp
		};
	}
}
