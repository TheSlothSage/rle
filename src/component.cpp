#include "component.hpp"
#include <stdexcept>



rle::component::Component::Component(std::string _name, lua_State* L) : name(_name) {
	std::string fullname = std::string("components." + name);

	luabridge::LuaRef component_table = luabridge::LuaRef::getGlobal(L, "components");
	luabridge::LuaRef component = component_table[name];
	if (component.isNil()) {
		throw std::runtime_error("rle::component::Component() : " + fullname + "; no component components table with name");
	}
	luabridge::LuaRef systems = component["systems"];
	if (systems.isNil()) {
		throw std::runtime_error("rle::component::Component() : " + fullname + ".systems does not exist");
	}

	std::string system_name;
	systems.push();
	lua_pushnil(L);
	while (lua_next(L, -2)) {
		if (lua_isstring(L, -1)) {
			system_name = luabridge::LuaRef::fromStack(L, -1).cast<std::string>();
		}
		else {
			throw std::runtime_error("rle::component::Component() : non string in system table");
		}

		system_table.push_back((system::System*)(new system::LuaSystem(L, system_name)));
		// pop value
		lua_pop(L, 1);
	}

	//pop key
	lua_pop(L, 1);

	// now load all of the data

	luabridge::LuaRef datatable = component["data"];
	if (datatable.isNil()) {
		throw std::runtime_error("rle::component::Component() : " + fullname + ".data does not exist");
	}


	std::string data_name;
	datatable.push();
	lua_pushnil(L);
	while (lua_next(L, -2)) {
		data_name = luabridge::LuaRef::fromStack(L, -2).cast<std::string>();
		comp_T v = std::monostate();
		auto ref = luabridge::LuaRef::fromStack(L, -1);
		switch (ref.type()) {
		case LUA_TNIL:
			throw std::runtime_error("Error in definition of component " + data_name + "; Cannot be nil");
			break;
		case LUA_TNUMBER:
			v = ComponentData<lua_Number>(ref.cast<lua_Number>());
			break;
		case LUA_TSTRING:
			v = ComponentData<std::string>(ref.cast<std::string>());
			break;
		case LUA_TBOOLEAN:
			v = ComponentData<bool>(ref.cast<bool>());
			break;
			// PLEASE ADD TABLES ONE DAY BECAUSE THEY KIND OF MATTER!
		default:
			throw std::runtime_error("Error in definition of component " + data_name + "; types can be strings, numbers and booleans");
			break;
		}
		data[data_name] = v;

		// pop value
		lua_pop(L, 1);
	}
	// pop key
	lua_pop(L, 1);
}

rle::component::Component::~Component(){
	
}

rle::component::BaseComponentData& rle::component::Component::Get (std::string _data){	
	try{
		return (BaseComponentData&)(data.at(_data));
	}
	catch(const std::out_of_range& e){
		throw  std::runtime_error("rle::component::Component::Get : Data member " + _data + " does not exist in component");
	}	
}

std::vector<rle::system::System*>& rle::component::Component::Systems(){
	return system_table;
}
