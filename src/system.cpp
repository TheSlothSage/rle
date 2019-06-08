#include "system.hpp"
#include <iterator>
#include <stdexcept>

rle::system::System::System(std::string _name){
	name = _name;
}

rle::system::System::~System(){ 	
	for(std::map<std::string, rle::system::Call*>::iterator i = call_objs.begin();
	    i != call_objs.end(); ++i){
		delete i->second;
	}
}

rle::system::Call::Call(std::string _name) : name(_name) {}
void rle::system::Call::Exec(){}

// At some point if we start adding more languages and other ways to interact 
// One could change this to inherit from a base Call object 
rle::system::LuaCall::LuaCall(lua_State* L_, std::string _system_name, std::string _name) : Call(_name), system_name(_system_name), L(L_){	
	luabridge::LuaRef system_table_ref = luabridge::getGlobal(L, "systems");
	luabridge::LuaRef system_ref = system_table_ref[system_name.c_str()];
	if(system_ref.isNil()){
		throw std::runtime_error("rle::system::LuaCall(): no system with name" + system_name + "exists in system table\n");
	}
	luabridge::LuaRef function = system_ref[Name().c_str()];
	if(function.isNil()){			
		throw std::runtime_error("rle::system::LuaCall() : function with name " + system_name + "." + _name + " does not exist");	       
	}
	if(!function.isFunction()){
			throw std::runtime_error("rle::system::LuaCall() : function with name " + system_name + "." + _name + " is not a function\n");
	}
}

void rle::system::LuaCall::Exec(){
	std::string lua_func_name = "systems." + system_name + "." + name; 
	luaL_dostring(L, std::string(lua_func_name + "()").c_str());
}

rle::system::LuaSystem::LuaSystem(lua_State* L_, std::string _name) : L(L_), system::System(_name){
	
	std::string fullname = "systems." + name;
	
	luabridge::LuaRef system_table = luabridge::LuaRef::getGlobal(L, "systems");
	luabridge::LuaRef system = system_table[name];
	if(system.isNil()){
		throw std::runtime_error("rle::component::System() : " + fullname + "; no system in systems table with name");
	}
	luabridge::LuaRef functions = system["functions"];
	if(functions.isNil()){
		throw std::runtime_error("rle::component::System() : " + fullname + ".functions table does not exist");
	}
	functions.push();
	lua_pushnil(L);
	while(lua_next(L, -2)){		
		if(lua_isstring(L, -1)){
			NewCall(std::string(lua_tostring(L,-1)), 0);
		}
		else{
			throw std::runtime_error("RLE::System::System() : " + fullname + ".functions contains something other than a string");
		}
		// pop value
		lua_pop(L,1);
	}	  
	// pop functions table
	lua_pop(L, 1);	
}

rle::system::LuaSystem::~LuaSystem(){
	
}

void rle::system::LuaSystem::NewCall(std::string key_str, unsigned int priority){
	if(priority > max_priority){
		max_priority = priority; 
	}
	call_objs.insert(std::make_pair(key_str, (Call*)(new rle::system::LuaCall(L, name, key_str)))); 
}

void rle::system::LuaSystem::DelCall(unsigned int index){
	call_objs.erase(std::next(call_objs.begin(), index)); // Not a random access iterator for some reason
}

void rle::system::LuaSystem::DelCall(std::string key_str){
	call_objs.erase(key_str); 
}

void rle::system::LuaSystem::ExecAll(){
	for (std::map<std::string, system::Call*>::iterator it = call_objs.begin(); it != call_objs.end(); ++it) {
		it->second->Exec();
	}
}

void rle::system::LuaSystem::ExecFunc(std::string key_str){
	try {
		Call* call = call_objs[key_str];
		if (call->enabled) {
			call->Exec();
		}
		return;
	}
	catch(std::exception& e){
		return;
	}
}
