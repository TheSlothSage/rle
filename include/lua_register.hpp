#pragma once 

extern "C"{ 
#include "math.h" 
}

#include "component.hpp"
#include "irle.hpp"

namespace rle{

	template<class container_T>
	class lua_Reg {
	protected:
		lua_Reg(rle::interface::basic_irle* rle_, container_T* container_) : rle(rle_), container(container_) { ++obj_count; }
		~lua_Reg() { --obj_count; }

		container_T* container;
		rle::interface::basic_irle* rle;

		static int obj_count;
	};

	namespace component {

		// maybe one day introduce some sort of cool way to have dynamic typing for components
		// but it seems to be failing horribly 

		struct lua_DataContainer {
			lua_DataContainer(int type_) : type(type_) {}
			~lua_DataContainer() = default;
			
			int getType() const { return type; }

			component::comp_T dat;
			const int type;
		};

		template<typename T>
		struct lua_Data : lua_DataContainer {						
			lua_Data(comp_T dat_) : lua_DataContainer(type) { dat = dat_; }
			~lua_Data() = default;

			T get() { throw std::runtime_error("invalid type"); }
			void set(T lval) { throw std::runtime_error("invalid_type"); }


			static int type;
		};

		using lua_Num = lua_Data<lua_Number>;
		using lua_Str = lua_Data<std::string>;
		using lua_Boo = lua_Data<bool>;

		class lua_Component : protected rle::lua_Reg<Component> {
		
		public:
			lua_Component(rle::interface::basic_irle* rle, Component* component) : lua_Reg(rle, component) {}
			~lua_Component() = default;

			std::string getStr(std::string key) {
				return container->Get_T<std::string>(key).getData();
			}
			lua_Number getNum(std::string key) {
				return container->Get_T<lua_Number>(key).getData();
			}
			bool getBool(std::string key) {
				return container->Get_T<bool>(key).getData();
			}

			void setStr(std::string key, std::string dat) { container->Get_T<std::string>(key).setData(dat); }
			void setNum(std::string key, lua_Number dat) { container->Get_T<lua_Number>(key).setData(dat); }
			void setBool(std::string key, bool dat) { container->Get_T<bool>(key).setData(dat); }

			std::string name() const { return container->Name(); }
		};

	}
	
	namespace entity {
		struct lua_Entity : protected rle::lua_Reg<Entity> {
			lua_Entity(rle::interface::basic_irle* rle, Entity* entity) : lua_Reg(rle, entity) {}
			~lua_Entity() = default;		

			int getTile_X() const { return container->X(); }
			int getTile_Y() const { return container->Y(); }
			int getTile_Z() const { return container->Z(); }

			void setTile_X(double x) { container->setX(::round(x)); }
			void setTile_Y(double y) { container->setY(::round(y)); }
			void setTile_Z(double z) { container->setZ(::round(z)); }

			component::lua_Component get(std::string key) {
				return component::lua_Component(rle, &rle->Instantiate<rle::entity::Entity>(*container).Do().Component(key));
			}
			std::string name() const {
				return container->Name();
			}
			lua_Entity& addComponent(std::string component) {
				rle->Instantiate<rle::entity::Entity>(*container).Do().addComponent(component);
				return *this;
			}
			lua_Entity& delComponent(std::string component) {
				rle->Instantiate<rle::entity::Entity>(*container).Do().delComponent(component);
				return *this;
			}
		};
	}
}
