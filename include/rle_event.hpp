#include "irle.hpp"

namespace rle {
	namespace game {
		
		struct IEvent {
			virtual void Do() = 0;
			inline static int eventId = -1;
		};

		template<

		struct CreateEntityEvent : public IEvent {
			virtual Do();
			rle::entity::Entity copy_ent;
			inline static int eventId = 1;
		};

		struct CreateComponentEvent : public IEvent {
			
			virtual Do();
			std::string entity, component;
			inline static int eventID = 2;
		};

		struct DeleteEntityEvent : public IEvent {
			DeleteComponentEvent(std::string entity_) : entity(entity_) {}
			virtual Do();
			std::string entity;
			inline static int eventID = 3;
		};

		struct DeleteComponentEvent : public IEvent {
			DeleteComponentEvent(std::string entity_, std::string component_) : entity(entity_), component(component_) {}
			virtual Do();
			std::string entity, component;
			inline static int eventID = 4;
		};
	}
}