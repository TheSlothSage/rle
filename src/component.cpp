#include "component.hpp"
#include <stdexcept>
rle::component::Component::Component(std::string _name, lua_State* L) : name(_name){
	// only for linux :(
	std::string fullname = std::string("components."+name);
	std::string pathname = std::string("components/"+name);
	if (luaL_loadfile(L, pathname.c_str()) ||
	    lua_pcall(L, 0, 0, 0)) {
		//there are many other ways this can fail btw
		throw std::runtime_error("rle::component::Component() : " + pathname + " either does not exist, has been moved, or script is somehow invalid. Returned with error:\n" + std::string(lua_tostring(L, -1))); 
	}
	luabridge::LuaRef component_table = luabridge::LuaRef::getGlobal(L, "components");
	luabridge::LuaRef component = component_table[name];
	if(component.isNil()){
		throw std::runtime_error("rle::component::Component() : " + fullname + "; no component components table with name");
	}
	luabridge::LuaRef systems = component["systems"];
	if(systems.isNil()){		
		throw std::runtime_error("rle::component::Component() : " + fullname + ".systems does not exist");
	}

	std::cout << "\t\t\tLoading System Dependencies..." << std::endl;
	
	std::string system_name;
	systems.push();	
	lua_pushnil(L);
	while(lua_next(L, -2)){
		if(lua_isstring(L, -1)){
			system_name = luabridge::LuaRef::fromStack(L, -1).cast<std::string>(); 
			std::cout << "\t\t\t\t-Loading " << system_name << std::endl;
		}
		else{			
			throw std::runtime_error("rle::component::Component() : non string in system table");
		}
		
		system_table.push_back((system::System*)(new system::LuaSystem(L,  system_name)));
		// pop value
		lua_pop(L,1);
	}
	
	//pop key
	lua_pop(L, 1);

	// now load all of the data

	luabridge::LuaRef datatable = component["data"];
	if(datatable.isNil()){
		throw std::runtime_error("rle::component::Component() : " + fullname + ".data does not exist");
	}

	std::cout << "\t\t\t Loading Component Data..." << std::endl;
	
	std::string data_name;
	datatable.push();
	lua_pushnil(L);
	while(lua_next(L, -2)){
		 data_name = luabridge::LuaRef::fromStack(L, -2).cast<std::string>();
		 data.insert(std::pair<std::string, luabridge::LuaRef>(data_name, luabridge::LuaRef::fromStack(L,-1)));
		 std::cout << "\t\t\t\t-" << data_name << std::endl;

		 // pop value
		 lua_pop(L, 1);
	}
	// pop key
	lua_pop(L, 1);
		
}

rle::component::Component::~Component(){
	
}

luabridge::LuaRef& rle::component::Component::Get (std::string _data){	
	try{
		return (data.at(_data));
	}
	catch(const std::out_of_range& e){
		throw  std::runtime_error("rle::component::Component::Get : Data member " + _data + " does not exist in component");
	}	
}

std::vector<rle::system::System*>& rle::component::Component::Systems(){
	return system_table;
}
