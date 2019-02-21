#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge.h"
#include <experimental/any>
#include <string>
#include <vector>
#include <map>
#include "system.hpp"

namespace rle{
	namespace component{

		class Component{
			std::string name;
			std::map<std::string, luabridge::LuaRef> data;
			luabridge::LuaRef* ret_data; 
			std::vector<system::System*> system_table;
			lua_State * L;
		public:
			Component(std::string _name, lua_State* _L);
			~Component();
			std::string Name(){ return name; }			
			Component& Get(std::string data);

			template<typename T>
			void Put(T& lval);
			void Put(luabridge::LuaRef& llua_ref);

			template<typename T>
			void Set(T& rval);
			template<typename T>
			void Set(luabridge::LuaRef& rlua_ref);
		};
	}
}
