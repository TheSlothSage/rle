#include "irle.hpp"

#define DEFAULT_IMPLEMENTOR_GET_SPECIALIZATION_STR(ret_type, from_type)template<> template<> interface::Specialized_Implementor<ret_type> interface::Default_Implementor<from_type, interface::Specialized_Implementor>::Get<ret_type>(std::string name) \

#define DEFAULT_IMPLEMENTOR_GET_SPECIALIZATION(ret_type, from_type) template<> template<> interface::Specialized_Implementor<ret_type> interface::Default_Implementor<from_type, interface::Specialized_Implementor>::Get<ret_type>() \

#define DEFAULT_IMPLEMENTOR_NAME_SPECIALIZATION(from_type) template<> template<> interface::Specialized_Implementor<std::string> interface::Default_Implementor<from_type, interface::Specialized_Implementor>::Name<std::string>() \

//Define Name() for everything that needs it
// macros would be nice here i guess

using namespace rle;

template<> template<>
interface::Specialized_Implementor<std::string>
interface::Default_Implementor<entity::Entity, interface::Specialized_Implementor>::Name<std::string>(){
	return Interface<std::string, interface::Specialized_Implementor>(*(new std::string(obj.Name()))).Do();	 
}

template<> template<>
interface::Specialized_Implementor<std::string>
interface::Default_Implementor<component::Component, interface::Specialized_Implementor>::Name<std::string>(){
	return Interface<std::string, interface::Specialized_Implementor>(*(new std::string(obj.Name()))).Do();
}

template<> template<>
interface::Specialized_Implementor<std::string>
interface::Default_Implementor<tile::TileMap, interface::Specialized_Implementor>::Name<std::string>(){
	return Interface<std::string, interface::Specialized_Implementor>(*(new std::string(obj.Name()))).Do();
}

// Define for Default_Implementor<RLE>
template<> template<> 
interface::Specialized_Implementor<rle::entity::Entity>
interface::Default_Implementor<rle::RLE, interface::Specialized_Implementor>::Get<entity::Entity>(std::string name){
	return Interface<entity::Entity, interface::Specialized_Implementor>(obj.GetEntity(name)).Do();
}

template<> template<>
interface::Specialized_Implementor<rle::system::System>
interface::Default_Implementor<rle::RLE, interface::Specialized_Implementor>::Get<system::System>(std::string name){
	return Interface<system::System, interface::Specialized_Implementor>(obj.GetSystem(name)).Do();
}
		
template<> template<>
interface::Specialized_Implementor<rle::tile::TileMap>
interface::Default_Implementor<rle::RLE, interface::Specialized_Implementor>::Get<tile::TileMap>(std::string name){
	return Interface<tile::TileMap, interface::Specialized_Implementor>(obj.GetTileMap(name)).Do();
}		

// define Default_Implementor<Entity> 

template<> template<>
interface::Specialized_Implementor<rle::component::Component>
interface::Default_Implementor<entity::Entity, interface::Specialized_Implementor>::Get<component::Component>(std::string name){
	return Interface<component::Component, interface::Specialized_Implementor>(obj.GetComponent(name)).Do();
}

// define Default_Implementor<Component>

template<> template<>
interface::Specialized_Implementor<luabridge::LuaRef>
interface::Default_Implementor<component::Component, interface::Specialized_Implementor>::Get<luabridge::LuaRef>(std::string name){
	return Interface<luabridge::LuaRef, interface::Specialized_Implementor>(obj.Get(name)).Do();
}

// define Default_Implementor<TileMap>

DEFAULT_IMPLEMENTOR_GET_SPECIALIZATION_STR(typename tile::Layer, typename tile::TileMap){
	return Interface<tile::Layer, interface::Specialized_Implementor>(obj.GetLayer(name)).Do();
}

// define system needs its own implementor since its not merely data, but calls as well 
		


