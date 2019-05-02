#include "gamecontroller.hpp"

/*
  Each widget must have its associated State
*/

// Initializes engine lua calls
class RegisterCall : Controller_Widget{
	virtual void Do();
	virtual void UpdateMembers(Base_Widget_State& state) {}
};
template<>
class Widget_State<RegisterCall>{
	Widget_State() {}
	void Sync() {} 
}
