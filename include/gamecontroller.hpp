#include "irle.hpp"

namespace rle{
	namespace game{
		class Controller_Widget;
		class GameController{
			interface::basic_irle irle;
			std::map<std::string, Controller_Widget*> widgets;
		public:
			GameController();
			interface::basic_irle& Iface() { return irle; }
			
			void LoadWidget(Controller_Widget& widget);
			void Run(std::string name); 
		};

		class Base_Widget_State {			
		protected:
			interface::basic_irle* irle = nullptr;

			Base_Widget_State() {}
			Base_Widget_State(interface::basic_irle& _irle) : irle(&_irle) {}  
			virtual ~Base_Widget_State() = default;
		public:
			void GiveIface(interface::basic_irle& _irle) { irle = &_irle; }
			virtual void Sync() = 0;
		};
		
		template<class widgetT>
		class Widget_State : Base_Widget_State{
		protected:
			Widget_State(interface::basic_irle& _irle) : Base_Widget_State(_irle) {}
			Widget_State() {}
			virtual ~Widget_State() = default;
		public:
			void Sync() {}
		};		
		     
		class Controller_Widget{
		protected:
			interface::basic_irle* irle = nullptr;
			std::string name;
			virtual void UpdateMembers(Base_Widget_State& state) = 0; 
		public:
			Controller_Widget(std::string _name, Base_Widget_State& state) : name(_name) { state.Sync(); Set(state); }
			std::string Name() { return name; } 
			virtual void Do() = 0;
			virtual void Set(Base_Widget_State& state) { state.GiveIface(*irle); UpdateMembers(state); }
		};
	}
}
