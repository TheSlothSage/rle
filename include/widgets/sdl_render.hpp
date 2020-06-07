#pragma once
#include "gamecontroller.hpp"
#include "SDLRender.hpp"

namespace rle {
	namespace game {
		namespace widgets {
			//required to run the game because it registers a bunch of really important stuff to lua
			
			class SDLRender : public Controller_Widget {
			public:
				SDLRender();
				bool register_functions = true;
				virtual void Do();
				virtual void UpdateMembers(Base_Widget_State& state) {};
				virtual std::string Name() { return "SDLRender"; }
			};
			
			template<>
			class Widget_State <SDLRender> : public Base_Widget_State{};
		}
	}
}
