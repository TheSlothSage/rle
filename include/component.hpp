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
#include <variant>
#include "system.hpp"

namespace rle{
	namespace component{ 

		struct BaseComponentData {
			BaseComponentData(int type_) : type(type_) {}
			int type;
		};

		template<class T>
		struct ComponentData : public BaseComponentData {			
			ComponentData(T data_) : BaseComponentData(type) { data = data_; }
			~ComponentData() = default;

			T getData() { throw std::runtime_error("ComponentData::getData : not a supported type"); }
			void setData(T data_) { throw std::runtime_error("ComponentData::setData : not a supported type");}
			
			static int type;
		
		private:
			T data;
		};

		lua_Number ComponentData<lua_Number>::getData() { return data; }
		std::string ComponentData<std::string>::getData() { return data; }
		bool ComponentData<bool>::getData() { return data; }

		void ComponentData<lua_Number>::setData(lua_Number number) { data = number; }
		void ComponentData<std::string>::setData(std::string str) { data = str; }
		void ComponentData<bool>::setData(bool boo) { data = boo; }

		

		typedef BaseComponentData Data;		
		template<class T>
		using T_Data = ComponentData<T>;
		using comp_T = std::variant<std::monostate, ComponentData<lua_Number>, ComponentData<std::string>, ComponentData<bool>>;
		
		template<class T> 
		int ComponentData<T>::type = -1; // error,  don't be this

		int ComponentData<lua_Number>::type = LUA_TNUMBER;
		int ComponentData<std::string>::type = LUA_TSTRING;
		int ComponentData<bool>::type = LUA_TBOOLEAN;

		class Component{
			std::string name;
			lua_State * L;						
		
		public:
			std::map<std::string, comp_T> data;
			std::vector<system::System*> system_table;
			Component(std::string name, lua_State* L);
			~Component();
			std::string Name(){ return name; }				       	
			
			Data& Get(std::string data_);
			
			template<typename T>
			T_Data<T>& Get_T(std::string data_) { 
				try {
					return std::get<ComponentData<T>>(data.at(data_));
				}
				catch (const std::bad_variant_access & e) {
					std::cerr << "\n\n\nAttempted to get component data member: '" + data_ + "' in component: '" + name + "' using the wrong type." << std::endl;
					std::cerr << "Aborting..." << std::endl;
				}
				catch (const std::out_of_range & e) {
					std::cerr << "error accessing '" + data_ + "' in component '" + Name() + "' ... No such member in component" << std::endl;;
				}
				catch (const std::runtime_error & e) {
					std::cerr << e.what() << std::endl;
				}
			}
			
			std::map<std::string, comp_T>::iterator begin() { return data.begin(); }
			std::map<std::string, comp_T>::iterator end() { return data.end(); }

			std::vector<system::System*>& Systems();
			void PopulateIntoRLEComponentTable(rle::RLE& rle); // in rle.cpp
		};
	}
}
