#include "critical_widget.hpp"
#include "lua_register.hpp"

rle::game::widgets::CriticalWidget::CriticalWidget() : Controller_Widget(Widget_State<CriticalWidget>()) {}

std::string GetThisEntityName() {
	return rle::game::EntityContext::entity_context;
}

rle::entity::lua_Entity GetEntity(std::string name) {
	try {
		rle::entity::Entity* ent = &(rle::game::Irle::irle->Do().Entity(name));	
		return rle::entity::lua_Entity(
			rle::game::Irle::irle,
			ent
		);
	}
	catch (std::exception& e) {
		std::cerr <<  e.what();
	}
}

rle::entity::lua_Entity GetThisEntity() {
	return rle::entity::lua_Entity(
		rle::game::Irle::irle,
		rle::game::EntityContext::entity_ptr
	);
}

rle::entity::lua_Entity NewEntity(std::string name, std::string tilemap_name, unsigned int x, unsigned int y, unsigned int z) {
	rle::interface::basic_irle& irle = *rle::game::Irle::irle;
	irle.Do().addEntity(tilemap_name, name, {}, x, y, z);
	return GetEntity(name);
}

void DelEntity(std::string name) {
	rle::interface::basic_irle& irle = *rle::game::Irle::irle;
	irle.Do().delEntity(name); 
	return;
}

void lua_LoadComponent(std::string name) {
	rle::interface::basic_irle& irle = *rle::game::Irle::irle;
	irle.Do().loadScript(std::string("components/" + name + ".lua"));
	
}

void lua_LoadSystem(std::string name) {
	rle::interface::basic_irle& irle = *rle::game::Irle::irle;
	irle.Do().loadScript(std::string("systems/" + name + ".lua"));
}

void lua_LoadScript(std::string path) {
	rle::interface::basic_irle& irle = *rle::game::Irle::irle;
	irle.Do().loadScript(path);
}

rle::tile::lua_TileMap NewTilemap(std::string name, unsigned int x, unsigned int y, unsigned int z) {
	rle::interface::basic_irle& irle = *rle::game::Irle::irle;
	irle.Do().addTilemap(name, x, y, z);
	return rle::tile::lua_TileMap(&irle, &(irle.Do().getTileMap(name)));
}

void DelTileMap(std::string name) {
	rle::interface::basic_irle& irle = *rle::game::Irle::irle;
	irle.Do().delTileMap(name); 
}

rle::tile::lua_TileMap getTilemap(std::string name){
    rle::interface::basic_irle& irle = *rle::game::Irle::irle;
    irle.Do().getTileMap(name);
} 

void rle::game::widgets::CriticalWidget::Do() {
	if (register_functions) {
		lua_State* L = irle->Do().LuaState();
		luabridge::getGlobalNamespace(L)
			.beginNamespace("rle")
				
				.beginClass<rle::component::lua_Component>("Component")
					.addProperty("name", &rle::component::lua_Component::name)
					.addFunction("getStr", &component::lua_Component::getStr)
					.addFunction("getNum", &component::lua_Component::getNum)
					.addFunction("getBool", &component::lua_Component::getBool)
					.addFunction("setStr", &component::lua_Component::setStr)
					.addFunction("setNum", &component::lua_Component::setNum)
					.addFunction("setBool", &component::lua_Component::setBool)
				.endClass()
				
				.beginClass<rle::entity::lua_Entity>("Entity")
					.addProperty("name", &entity::lua_Entity::name)
					.addProperty("x", &entity::lua_Entity::getTile_X, &entity::lua_Entity::setTile_X)
					.addProperty("y", &entity::lua_Entity::getTile_Y, &entity::lua_Entity::setTile_Y)
					.addProperty("z", &entity::lua_Entity::getTile_Z, &entity::lua_Entity::setTile_Z)			
					.addFunction("setTileMap",  &entity::lua_Entity::setTileMap)
					.addFunction("getTileMap", &entity::lua_Entity::getTileMap)
					.addFunction("getComponent", &entity::lua_Entity::get)
					.addFunction("addComponent", &entity::lua_Entity::addComponent)
					.addFunction("delComponent", &entity::lua_Entity::delComponent)
					.addFunction("checkComponent", &entity::lua_Entity::checkComponent)
				.endClass()
			
				.beginClass<rle::tile::lua_Tile>("Tile")
					.addFunction("getEntityByName", &tile::lua_Tile::getEntityByName)
					.addFunction("getEntity", &tile::lua_Tile::getEntity)
					.addProperty("entities", &tile::lua_Tile::entityCount)
				.endClass()
				        
				.beginClass<rle::tile::lua_TileMap>("TileMap")
					.addProperty("name", &tile::lua_TileMap::Name)
				   	.addProperty("x", &tile::lua_TileMap::mapX)
					.addProperty("y", &tile::lua_TileMap::mapY)
					.addProperty("z", &tile::lua_TileMap::mapZ) 
					.addFunction("getTile", &tile::lua_TileMap::tile)
				.endClass()

				.addFunction("entityName", &GetThisEntityName)
				.addFunction("this", &GetThisEntity)
				.addFunction("getEntity", &GetEntity)
				.addFunction("newEntity", &NewEntity)	
				.addFunction("delEntity", &DelEntity)
				.addFunction("newTilemap", &NewTilemap)
				.addFunction("getTilemap", &getTilemap)
				.addFunction("loadComponent", &lua_LoadComponent)
				.addFunction("loadSystem", &lua_LoadSystem)
				.addFunction("loadScript", &lua_LoadScript)
			.endNamespace();
		// generates tables in lua with proxies for various component data

		register_functions = false;
	}
}
