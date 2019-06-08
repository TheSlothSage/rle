#include "irle.hpp"

using namespace rle;

#define DEFAULT_IMPLEMENTOR_GET_SPECIALIZATION_STR(ret_type, from_type)template<> template<> interface::Specialized_Implementor<ret_type>& interface::Default_Implementor<from_type>::Get<ret_type>(std::string name) \

#define DEFAULT_IMPLEMENTOR_GET_SPECIALIZATION(ret_type, from_type) template<> template<> interface::Specialized_Implementor<ret_type>& interface::Default_Implementor<from_type>::Get<ret_type>() \

#define DEFAULT_IMPLEMENTOR_NAME_SPECIALIZATION(from_type) template<> template<> interface::Specialized_Implementor<std::string>& interface::Default_Implementor<from_type>::Name<std::string>() \

// These two macros require that the user supply the args since one cannot know what all the possible args might be
// DS* refers to DOUBLE SPECIALIZATION, while SS* stands for SINGLE SPECIALIZATION. Methods using macros without extending macro functionality must be specialized by providing the template argument after invocation of macro. See examples below. 

#define DS_DEFAULT_IMPLEMENTOR_SPECIALIZATION_FROM_METHOD(ret_type, from_type, method_name) template<> template<> interface::Specialized_Implementor<ret_type>& interface::Default_Implementor<from_type>::method_name

#define DS_SPECIALIZED_IMPLEMENTOR_SPECIALIZATION_FROM_METHOD(ret_type, from_type, method_name) template<> template<> interface::Specialized_Implementor<ret_type>& interface::Specialized_Implmentor<from_type>::method_name \s

#define SS_DEFAULT_IMPLEMENTOR_SPECIALIZATION_FROM_METHOD(ret_type, from_type, method_name) template<> interface::Specialized_Implementor<ret_type>& interface::Specialized_Implementor<from_type>::method_name \
       
#define SS_SPECIALIZED_IMPLEMENTOR_SPECIALIZE_FROM_METHOD(ret_type, from_type, method_name) template<> interface::Specialized_Implementor<ret_type>& interface::Specialized_Implementor<from_type>::method_name \

// Define Name() for everything that needs it

DEFAULT_IMPLEMENTOR_NAME_SPECIALIZATION(entity::Entity) {
	return Instantiate_ByVal<std::string>(obj.Name()).Do(); 
}

DEFAULT_IMPLEMENTOR_NAME_SPECIALIZATION(component::Component){
	return Instantiate_ByVal<std::string>(obj.Name()).Do();
}

DEFAULT_IMPLEMENTOR_NAME_SPECIALIZATION(tile::TileMap){
	return Instantiate_ByVal<std::string>(obj.Name()).Do();
}

// Define for Default_Implementor<RLE>

DEFAULT_IMPLEMENTOR_GET_SPECIALIZATION_STR(entity::Entity, RLE){
	return Instantiate<entity::Entity>(obj.GetEntity(name)).Do(); 
}

DEFAULT_IMPLEMENTOR_GET_SPECIALIZATION_STR(system::System, RLE){
	return Instantiate<system::System>(obj.GetSystem(name)).Do(); 
}

DEFAULT_IMPLEMENTOR_GET_SPECIALIZATION_STR(tile::TileMap, RLE){
	return Instantiate<tile::TileMap>(obj.GetTileMap(name)).Do();
}

// define Default_Implementor<Entity> 

DEFAULT_IMPLEMENTOR_GET_SPECIALIZATION_STR(component::Component, entity::Entity){
	return Instantiate<component::Component>(obj.GetComponent(name)).Do(); 
}

// define Default_Implementor<Component>

DEFAULT_IMPLEMENTOR_GET_SPECIALIZATION_STR(rle::component::Data, component::Component){
	return Instantiate<rle::component::Data>(obj.Get(name)).Do();
}

// define Default_Implementor<TileMap>

DEFAULT_IMPLEMENTOR_GET_SPECIALIZATION_STR(tile::Layer, tile::TileMap){
	return Instantiate<tile::Layer>(obj.GetLayer(name)).Do();
}

// define system needs its own implementor since its not merely data, but calls as well 
		


