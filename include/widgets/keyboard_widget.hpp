#pragma once
#include "gamecontroller.hpp"

namespace rle {
	namespace game {
		namespace widgets {
			class KeyboardWidget : public Controller_Widget {
			public:
				KeyboardWidget();
				bool register_functions = true;
				virtual void Do();
				virtual void UpdateMembers(Base_Widget_State& state) {};
				virtual std::string Name() { return "Keyboard_Widget"; }
			};

			template<>
			class Widget_State<KeyboardWidget> : public Base_Widget_State {};
		}
	}
}

