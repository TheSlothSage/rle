#pragma once

#include "irle.hpp"

namespace rle{
	namespace game{		

		struct EntityContext {
		public:
			EntityContext() {}
			~EntityContext() {}
			static std::string entity_context;
			static entity::Entity* entity_ptr;
		};

		struct Irle {
		public:
			static rle::interface::basic_irle* irle;
		};

		class Controller_Widget;

		class GameController{
			interface::basic_irle irle;
			std::map<std::string, Controller_Widget*> widgets;
			
			double delta_t = 60.0;
			
			// if there are mutliple rle instances up, it would be wise to use instance ids
			
			const int instanceID ;

		public:
			GameController(const int instance) : instanceID(instance) {}
			GameController() : instanceID(0) {}
			interface::basic_irle& Iface() { return irle; }
			std::string current_obj_name;
			void LoadWidget(Controller_Widget& widget);
			void Run(std::string name); 
			void RunAll();
			void Start();
			void Step();
			void Bootstrap();
			void ExecCallAll(std::string call_name);
		};

		class Base_Widget_State {			
		protected:
			interface::basic_irle* irle = nullptr; 
		public:
			Base_Widget_State() {}
			Base_Widget_State(interface::basic_irle& _irle) : irle(&_irle) {}
			void GiveIface(interface::basic_irle& _irle) { irle = &_irle; }
			void Sync() {}
		};


		namespace widgets {
			template<class widgetT>
			class Widget_State : public Base_Widget_State {
			public:
				Widget_State(interface::basic_irle& _irle) : Base_Widget_State(_irle) {}
				Widget_State() {}
				void Sync() {}
			};
		}
		

		class Controller_Widget {
		protected:
			virtual void UpdateMembers(Base_Widget_State& state) {};
			interface::basic_irle* irle = nullptr;
		public:

			Controller_Widget() {}
			Controller_Widget(Base_Widget_State state) { Set(state); state.Sync();  }
			virtual std::string Name() { return ""; };
			virtual void Do() {}
			void Set(Base_Widget_State& state) { state.GiveIface(*irle);  UpdateMembers(state); }
			void SetInterface(interface::basic_irle* irle_) { irle = irle_; }
		};
	}
}
