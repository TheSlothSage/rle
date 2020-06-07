#include "gamecontroller.hpp"
#include "lua_register.hpp"
#include "SDL.h"

#include <chrono>


std::string rle::game::EntityContext::entity_context = "undefined";
rle::entity::Entity* rle::game::EntityContext::entity_ptr = nullptr;

rle::interface::basic_irle* rle::game::Irle::irle = nullptr; 

void rle::game::GameController::LoadWidget(Controller_Widget& widget) {
	widget.SetInterface(&(this->irle));
	for (std::map<std::string, Controller_Widget*>::iterator it = widgets.begin(); it != widgets.end(); ++it) {
		if (it->second->Name() == widget.Name()) {
			std::cerr << "WARNING; rle::game::GameController::LoadWidget:\n\twidget already loaded\n: " << widget.Name() << std::endl;
			return;
		}
	}
	widgets[widget.Name()] = &widget;
}

void rle::game::GameController::RunAll() {
	for (std::map<std::string, rle::game::Controller_Widget*>::iterator it = widgets.begin(); it != widgets.end(); ++it) {
		it->second->Do();
	}	
}

void rle::game::GameController::Run(std::string name) {
	widgets[name]->Do();		
}	

void rle::game::GameController::ExecCallAll(std::string call_name) {	
	for (std::map<std::string, rle::entity::Entity*>::iterator ent = irle.Do().getEntityTable().begin(); ent != irle.Do().getEntityTable().end(); ++ent) {
		EntityContext::entity_context = ent->first;
		EntityContext::entity_ptr = ent->second;
		for (std::vector<rle::component::Component*>::reverse_iterator comp = ent->second->component_table.rbegin(); comp != ent->second->component_table.rend(); ++comp ) {
			for (system::System* sys : (*comp)->system_table) {
				std::cout << call_name << std::endl;
				sys->ExecFunc(call_name);									
			}			
		}
	}
}

void rle::game::GameController::Bootstrap() {
	RunAll();
	try {
		irle.Do().loadScript("rle.lua");
	}
	catch (std::exception & e) {
		std::cerr << e.what();
	}
	
}

void rle::game::GameController::Start() {
	
	Irle::irle = &irle;
		
	Bootstrap();
	
	// Enter the startup state and start initializing things such as tilemaps and entities
	auto start = std::chrono::high_resolution_clock::now();

	while (true) {
	        RunAll();
		ExecCallAll("Init");
		ExecCallAll("Update");
	}
		/*  -- time stuff --
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> delta = end - start;
		if (delta.count() < 1/delta_t) {
			sleep(delta.count() - 1 / delta_t);
		}
		*/

	if (lua_gettop(irle.Do().LuaState()) > 0) {
		std::cout << lua_tostring(irle.Do().LuaState(), -1) << std::endl;
	}

	lua_settop(irle.Do().LuaState(), -1);

}



