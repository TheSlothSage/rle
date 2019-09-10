#include "lua_register.hpp"

template<> int rle::lua_Reg<rle::component::Component>::obj_count = 0;
template<> int rle::lua_Reg<rle::entity::Entity>::obj_count = 0;

template<> int rle::component::lua_Num::type = rle::component::ComponentData<lua_Number>::type;
template<> int rle::component::lua_Str::type = rle::component::ComponentData<std::string>::type;
template<> int rle::component::lua_Boo::type = rle::component::ComponentData<bool>::type;

template<> lua_Number rle::component::lua_Num::get() { return std::get<1>(dat).getData(); }
template<> std::string rle::component::lua_Str::get() { return std::get<2>(dat).getData(); }
template<> bool rle::component::lua_Boo::get() { return std::get<3>(dat).getData(); }

template<> void rle::component::lua_Num::set(lua_Number lval) { std::get<1>(dat).setData(lval); }
template<> void rle::component::lua_Str::set(std::string lval) { std::get<2>(dat).setData(lval); }
template<> void rle::component::lua_Boo::set(bool lval) { std::get<3>(dat).setData(lval); }


