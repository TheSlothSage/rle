#pragma once
#include "gamecontroller.hpp"

// Initializes engine lua calls

namespace rle {
	namespace game {
		namespace widgets {
			class DebugCall : public Controller_Widget {
			public:
				DebugCall();
				bool register_functions = true;
				virtual void Do();
				virtual void UpdateMembers(Base_Widget_State& state) {};
				virtual std::string Name() { return "Register_Debug_Calls"; }
			};

			template<>
			class Widget_State<DebugCall> : public Base_Widget_State {};
		}
	}
}	

