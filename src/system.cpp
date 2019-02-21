#include "system.hpp"
#include <iterator>
#include <stdexcept>

rle::system::System::System(lua_State* _L, std::string _name){
	L = _L;
	name = _name;
	
	
}

rle::system::System::~System(){
	// pure virtual destructor requires definition 
}

rle::system::Call::Call(std::string _name) : name(_name){}
void rle::system::Call::Exec() {}

// At some point if we start adding more languages and other ways to interact 
// One could change this to inherit from a base Call object 
rle::system::LuaCall::LuaCall(lua_State* L, std::string _system_name, std::string _name) : Call(_name), system_name(_system_name){
	luabridge::LuaRef system_table_ref = luabridge::getGlobal(L, "system");
	luabridge::LuaRef system_ref = system_table_ref[system_name.c_str()];
	if(system_ref.isNil()){
		throw std::runtime_error("rle::system::LuaCall(): no system with name" + system_name + "exists in system table\n");
	}
	if(!system_ref[name].isFunction()){			
		throw std::runtime_error("rle::system::LuaCall() : function with name " + _name + " either does not exist or is not a function\n"); 
	}			
}

void rle::system::LuaCall::Exec(){

}

rle::system::LuaSystem::LuaSystem(lua_State* _L, std::string _name) : system::System(_L, _name){
	
}

rle::system::LuaSystem::~LuaSystem(){
	
}

void rle::system::LuaSystem::NewCall(const std::string key_str, const unsigned int priority){
	if(priority > max_priority){
		max_priority = priority; 
	}
	call_objs.insert(std::make_pair(key_str, (Call*)(new rle::system::LuaCall(L, key_str))); 
}

void rle::system::LuaSystem::DelCall(unsigned int index){
	call_objs.erase(std::next(call_objs.begin(), index)); // Not a random access iterator for some reason
}

void rle::system::LuaSystem::DelCall(std::string key_str){
	call_objs.erase(key_str); 
}

void rle::system::LuaSystem::ExecAll(){
	// Do lua shtuff here
}

void rle::system::LuaSystem::ExecFunc(std::string key_str){
	// Do lua shtuff here 
}
