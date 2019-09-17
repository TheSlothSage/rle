#include "lua_register.hpp"

template<> int rle::lua_Reg<rle::component::Component>::obj_count = 0;
template<> int rle::lua_Reg<rle::entity::Entity>::obj_count = 0;
template<> int rle::lua_Reg<rle::tile::TileMap>::obj_count = 0;
template<> int rle::lua_Reg<rle::tile::Tile>::obj_count = 0;

template<> int rle::component::lua_Num::type = rle::component::ComponentData<lua_Number>::type;
template<> int rle::component::lua_Str::type = rle::component::ComponentData<std::string>::type;
template<> int rle::component::lua_Boo::type = rle::component::ComponentData<bool>::type;

template<> lua_Number rle::component::lua_Num::get() { return std::get<1>(dat).getData(); }
template<> std::string rle::component::lua_Str::get() { return std::get<2>(dat).getData(); }
template<> bool rle::component::lua_Boo::get() { return std::get<3>(dat).getData(); }

template<> void rle::component::lua_Num::set(lua_Number lval) { std::get<1>(dat).setData(lval); }
template<> void rle::component::lua_Str::set(std::string lval) { std::get<2>(dat).setData(lval); }
template<> void rle::component::lua_Boo::set(bool lval) { std::get<3>(dat).setData(lval); }

// lua_Entity must be defined for these functions to work

rle::entity::lua_Entity rle::tile::lua_Tile::getEntityByName(std::string name){
    
    for(rle::entity::Entity*& ent : container->entities){
        if(ent->Name() == name) { return rle::entity::lua_Entity(rle, ent); }
    }        

    throw std::runtime_error("lua_Tile::getEntityByName : entity with name " + name + " is not contained in Tile");
}

rle::entity::lua_Entity rle::tile::lua_Tile::getEntity(unsigned int index) {

    if( index < container->entities.size() ){
        return entity::lua_Entity(rle, &(*container)[index]); 
    }

    else{
        throw std::runtime_error("lua_Tile::getEntity : index out of range");
    }
}
