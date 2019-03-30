#pragma once

#include "rle.hpp"
#include "component.hpp"
#include "system.hpp"
#include "tilemap.hpp"
#include "entity.hpp"

#include <experimental/any>

namespace rle{
	namespace interface{
	       		
		template<class T, template<class> class T_Implementor>
		class Interface{
		protected:		
			T_Implementor<T>& impl;			
		public:
			Interface() : impl(*(new T_Implementor<T>(*(new T)))) {}
			Interface(T& obj) : impl(*(new T_Implementor<T>(obj))) {}
			T_Implementor<T>& Do() { return impl; }
		};

		template<class T>
		class Default_Implementor{
		protected:
			std::experimental::any rbuf;
			T& obj;
		public:
			Default_Implementor() : obj(*(new T())) {}
			Default_Implementor(T& _obj) : obj(_obj) {}
			template<class getType>
			Default_Implementor<getType> Get(std::string name) {
				throw std::runtime_error("Implementor::Get : No such table with member type given with args std::string name");
			}
			template<class getType>
			Default_Implementor<getType> Get(){
				throw std::runtime_error("Implementor::Get : No such table with member type given with no args");	
			}
			template<class strType = std::string>
			Default_Implementor<strType> Name() {
				throw std::runtime_error("Implementor::Name : Object with this interface does not have Name() defined for it"); 
			}
			
			T& GiveInstance() { return obj; }
			std::experimental::any& GiveBuf() { return rbuf; }
		};

		template<typename T>
		using basic_Interface = Interface<T, Default_Implementor>;
		using basic_irle = Interface<rle::RLE, Default_Implementor>;

		//Define Name() for everything that needs it
		// macros would be nice here i guess
		template<> template<>
		Default_Implementor<std::string>
		Default_Implementor<entity::Entity>::Name<std::string>(){
			return Interface<std::string,Default_Implementor>(*(new std::string(obj.Name()))).Do();				
		}

		template<> template<>
		Default_Implementor<std::string>
		Default_Implementor<component::Component>::Name<std::string>(){
			return Interface<std::string,Default_Implementor>(*(new std::string(obj.Name()))).Do();
		}

		template<> template<>
		Default_Implementor<std::string>
		Default_Implementor<tile::TileMap>::Name<std::string>(){
			return Interface<std::string,Default_Implementor>(*(new std::string(obj.Name()))).Do();
		}
				
		// Define for Default_Implementor<RLE>
		template<> template<> 
		Default_Implementor<rle::entity::Entity>
		Default_Implementor<rle::RLE>::Get<entity::Entity>(std::string name){
			return Interface<entity::Entity, Default_Implementor>(obj.GetEntity(name)).Do();
		}

		
		template<> template<>
		Default_Implementor<rle::system::System>
		Default_Implementor<rle::RLE>::Get<system::System>(std::string name){
			return Interface<system::System, Default_Implementor>(obj.GetSystem(name)).Do();
		}
		
		template<> template<>
		Default_Implementor<rle::tile::TileMap>
		Default_Implementor<rle::RLE>::Get<tile::TileMap>(std::string name){
			return Interface<tile::TileMap, Default_Implementor>(obj.GetTileMap(name)).Do();
		}		

		// define Default_Implementor<Entity> 
		
		template<> template<>
		Default_Implementor<rle::component::Component>
		Default_Implementor<entity::Entity>::Get<component::Component>(std::string name){
			return Interface<component::Component, Default_Implementor>(obj.GetComponent(name)).Do();
		}
	}
}
