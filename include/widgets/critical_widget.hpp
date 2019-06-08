#pragma once
#include "gamecontroller.hpp"

namespace rle {
	namespace game {
		namespace widgets {
			//required to run the game because it registers a bunch of really important stuff to lua
			
			class CriticalWidget : public Controller_Widget {
			public:
				CriticalWidget();
				bool register_functions = true;
				virtual void Do();
				virtual void UpdateMembers(Base_Widget_State& state) {};
				virtual std::string Name() { return "Register_Critical_Calls"; }
			};
			
			template<>
			class Widget_State <CriticalWidget> : public Base_Widget_State{};
		}
	}
}
