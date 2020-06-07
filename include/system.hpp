#pragma once
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
 }
#include "LuaBridge.h"
#include <vector>
#include <string>
#include <cstring>
#include <variant> 
#include <map>
#include <sstream>
namespace rle {
	struct RLE;
	namespace system{

		class Call{
		protected:
			std::string name;
		public:
			bool enabled = true; // if disabled it won't call until renabled manually
			Call(std::string _name);
			std::string Name(){
				return name;
			}
			virtual void Exec();
		};
		class LuaCall : Call{
			std::string system_name;
			lua_State* L;
		public:
			LuaCall(lua_State* L, std::string _system_name, std::string _name);	
			void Exec(); 
		};
		
		class System{
		protected:
			std::string name;
			unsigned int max_priority = 0;
			std::map<std::string, Call*> call_objs;
		public:
			System(std::string _name);
			virtual ~System() = 0;
			std::string& Name(){
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
			void PopulateIntoRLESystemTable(rle::RLE& rle); // in rle.cpp
		};

		class LuaSystem :  System{
		protected:
			lua_State* L;
		public:	
			LuaSystem(lua_State* L, std::string name);
			~LuaSystem();  
			void NewCall(std::string str, unsigned int priority);
			void DelCall(unsigned int index);
			void DelCall(std::string key_str); 
			void ExecAll();
			void ExecFunc(std::string key_str); 
		};
	}
}
