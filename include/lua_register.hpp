#pragma once 

#include "component.hpp"

namespace rle{

	template<class container_T>
	class lua_Reg {
	protected:
		lua_Reg(interface::basic_irle* rle_, container_T* container_) : rle(rle_), container(container_) { ++obj_count; }
		~lua_Reg() { --obj_count; }

		container_T* container;
		interface::basic_irle* rle;

		static int obj_count;
	};

	template<class T>
	int lua_Reg<T>::obj_count = 0;
	int lua_Reg<rle::component::Component>::obj_count = 0;
	int lua_Reg<rle::entity::Entity>::obj_count = 0;

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

		int lua_Num::type = ComponentData<lua_Number>::type;
		int lua_Str::type = ComponentData<std::string>::type;
		int lua_Boo::type = ComponentData<bool>::type;

		template<> lua_Number lua_Num::get() { return std::get<1>(dat).getData(); }
		template<> std::string lua_Str::get() { return std::get<2>(dat).getData(); }
		template<> bool lua_Boo::get() { return std::get<3>(dat).getData(); }

		template<> void lua_Num::set(lua_Number lval) { std::get<1>(dat).setData(lval); }
		template<> void lua_Str::set(std::string lval) { std::get<2>(dat).setData(lval); }
		template<> void lua_Boo::set(bool lval) { std::get<3>(dat).setData(lval); }


		class lua_Component : protected rle::lua_Reg<Component> {
		
		public:
			lua_Component(interface::basic_irle* rle, Component* component) : lua_Reg(rle, component) {}
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
			lua_Entity(interface::basic_irle* rle, Entity* entity) : lua_Reg(rle, entity) {}
			~lua_Entity() = default;		

			int getTile_X() const { return container->X(); }
			int getTile_Y() const { return container->Y(); }
			int getTile_Z() const { return container->Z(); }

			void setTile_X(double x) { container->setX(std::round(x)); }
			void setTile_Y(double y) { container->setY(std::round(y)); }
			void setTile_Z(double z) { container->setZ(std::round(z)); }

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
