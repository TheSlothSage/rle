#include "component.hpp"
#include <stdexcept>
rle::component::Component::Component(std::string _name, lua_State* L) : name(_name){
	// only for linux :(
	std::string fullname = std::string("components."+_name);
	std::string pathname = std::string("components/"+_name);
	if (luaL_loadfile(L, pathname.c_str()) ||
	    lua_pcall(L, 0, 0, 0)) {
		//there are many other ways this can fail btw
		throw std::runtime_error("rle::component::Component() : components/" + name + " either does not exist or has been moved"); 
	}
	luabridge::LuaRef component_table = luabridge::LuaRef::getGlobal(L, "components");
	luabridge::LuaRef component = component_table[name];
	if(component.isNil()){
		throw std::runtime_error("rle::component::Component() : " + fullname + "; no component with name");
	}
	luabridge::LuaRef systems = component["systems"];
	if(systems.isNil()){		
		throw std::runtime_error("rle::component::Component() : " + fullname + ".systems does not exist");
	}
	systems.push();
	std::string system_name;
	lua_pushnil(L);
	while(lua_next(L, -2)){
		if(lua_isstring(L, -1)){
			system_name = luabridge::LuaRef::fromStack(L, -1).cast<std::string>(); 
		}
		else{			
			throw std::runtime_error("rle::component::Component() : non string in system table");
		}
		system_table.push_back((system::System*)(new system::LuaSystem(L,  system_name)));
		lua_pop(L,1);
	}
	//pop the key
	lua_pop(L,1);
	//pop system table
	lua_pop(L, 1);
}

rle::component::Component::~Component(){
	for(system::System*& s : system_table){
		delete s;
	}
}

rle::component::Component& rle::component::Component::Get(std::string _data){	
	try{
		ret_data = &(data.at(_data));
	}
	catch(const std::out_of_range& e){
		const char * c_data = _data.c_str();
		luabridge::LuaRef ref(L, c_data);
		data.insert(std::pair<std::string, luabridge::LuaRef>(_data, ref));
		ret_data = &(data.at(_data));
	}
	return *this;
}

template<typename T>
void rle::component::Component::Put(T& lval){
	lval = ret_data->cast<T>();
}

void rle::component::Component::Put(luabridge::LuaRef& llua_ref){
	llua_ref = *ret_data;
}

template<typename T>
void rle::component::Component::Set(T& rval){
	*ret_data(L, rval);
}

template<typename T>
void rle::component::Component::Set(luabridge::LuaRef& rlua_ref){
	ret_data(L, rlua_ref.cast<T>());
}
