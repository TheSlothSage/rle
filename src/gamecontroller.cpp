#include "gamecontroller.hpp"
#include "lua_register.hpp"

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
	unsigned int ent_size = irle.Do().getEntityTable().size();
	for (unsigned int i = 0; i < ent_size; ++i) {
		auto ent = irle.Do().getEntityTable().at(i);
		EntityContext::entity_context = ent->Name();
		EntityContext::entity_ptr = ent;
		for (component::Component*& comp : ent->component_table) {
			for (system::System* sys : comp->system_table) {
				if (ent->callInit && call_name != "Init") { sys->ExecFunc("Init"); }
				sys->ExecFunc(call_name);									
			}			
		}
		// account for the table growing or shrinking
		unsigned int ent_size = irle.Do().getEntityTable().size();
		// for some reason ent pointer dies a terrible death. i assume because the stack frame dies. who knows?
		EntityContext::entity_ptr->callInit = false;
	}
}

void rle::game::GameController::Bootstrap() {
	irle.Do().initLua();
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
