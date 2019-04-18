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
		class Specialized_Implementor;
		
		template<class T, template<class> class Ret_Implementor>
		class Default_Implementor{
		protected:
			T& obj;
		public:
			Default_Implementor() : obj(*(new T())) {}
			Default_Implementor(T& _obj) : obj(_obj) {}
			template<class getType>
			Ret_Implementor<getType> Get(std::string name) {
				throw std::runtime_error("Implementor::Get() : No such table with member type given with args std::string name");
			}
			template<class getType>
			Ret_Implementor<getType> Get(){
				throw std::runtime_error("Implementor::Get() : No such table with member type given with no args");	
			}
			template<class strType = std::string>
			Ret_Implementor<strType> Name() {
				throw std::runtime_error("Implementor::Name() : Object with this interface does not have Name() defined for it"); 
			}

			template<typename setType>
			Ret_Implementor<T> Set(std::string name, setType& rval) {
				throw std::runtime_error("Implementor::Set() : Object with this interface does not have Set() defined for it"); 				
			};
			
			T& GiveInstance() { return obj; }
		};
		
		template<class T>
		class Specialized_Implementor : public Default_Implementor<T, Specialized_Implementor>{
		public:
			Specialized_Implementor() : Default_Implementor<T, Specialized_Implementor>() {}
			Specialized_Implementor(T& _obj) : Default_Implementor<T, Specialized_Implementor>(_obj) {} 
		};

		// Specialization for tile::Layer
		
		template<>
		class Specialized_Implementor<tile::Layer> : public Default_Implementor<tile::Layer, Specialized_Implementor>{
		private:
			template<typename T>
			tile::Derived_Layer<T>& DownCastToDerived(){
				tile::Derived_Layer<T>* dl = dynamic_cast<tile::Derived_Layer<T>*>(this);
				if(dl){
					return *this;
				}
				throw std::runtime_error("Implementor for tile::Layer : A dynamic cast failed from Layer -> Derived_Layer (Is the template argument the correct type for the layer?"); 
			}
		public:
			Specialized_Implementor<tile::Layer>(tile::Layer& _obj) : Default_Implementor<tile::Layer, Specialized_Implementor>(_obj) {}
			template<typename T>
			Specialized_Implementor<T> Data(unsigned int){
				 
			}
			template<typename T>
			Specialized_Implementor<T> Data(unsigned int x, unsigned int y, unsigned int z){
				 
			}
		};

		template<class T>
		using basic_interface = Interface<T, Specialized_Implementor>;
		using basic_irle = Interface<RLE, Specialized_Implementor>;						
	}
}
