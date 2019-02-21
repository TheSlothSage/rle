#pragma once
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge.h"
#include <vector>
#include <string>
#include <cstring>
#include <variant> 
#include <map>
#include <sstream>
namespace rle {
	namespace system{

		class Call{
			std::string name;
		public:
			Call(std::string _name);
			std::string Name(){
				return name;
			}
			virtual void Exec();
		};
		class LuaCall : Call{
			std::string system_name;
			std::string name;
		public:
			LuaCall(lua_State* L, std::string _system_name, std::string _name); 		       
			void Exec(); 
		};
		
		class System{
		protected:
			std::string name;
			lua_State* L;
			unsigned int max_priority = 0;
			std::map<std::string, Call*> call_objs;
		public:
			System(lua_State* _L, std::string _name);
			virtual ~System() = 0;
			std::string Name(){
				return name;
			}
			void C_Name(char** _name){
				*_name = new char[name.length()+1];
				std::strcpy(*_name, name.c_str());
			}
			
			virtual void NewCall(std::string key_str, unsigned int priority) = 0;
			virtual void DelCall(unsigned int index) = 0;
			virtual void DelCall(std::string key_str) = 0; 
			virtual void ExecAll() = 0;
			virtual void ExecFunc(std::string key_str) = 0;
		};

		class LuaSystem :  System{
		public:	
			LuaSystem(lua_State* _L, std::string _name);
			~LuaSystem();
			void NewCall(std::string str, unsigned int priority);
			void DelCall(unsigned int index);
			void DelCall(std::string key_str); 
			void ExecAll();
			void ExecFunc(std::string key_str); 
		};
	}
}
