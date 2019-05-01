#pragma once

#include "rle.hpp"
#include "component.hpp"
#include "system.hpp"
#include "tilemap.hpp"
#include "entity.hpp"

namespace rle{
	namespace interface{
		
		template<class T_Carry>
		struct IfaceCarry{			
			IfaceCarry(T_Carry& _carry_obj) : carry_obj(&_carry_obj) {}
			IfaceCarry() {}
			T_Carry* carry_obj;
		};
		
		template<class T, template<class> class T_Implementor, class T_Carry>
		class Base_Interface : IfaceCarry<T_Carry>{
		private:
			T* obj_ptr;
			
			void __TryInitCarry(){
				bool throw_runtime_error = false;
				try{
					this->carry_obj = obj_ptr;
				}catch(std::exception& e){
					throw_runtime_error = true;
				}
				if(throw_runtime_error){
					throw std::runtime_error("interface::Base_Interface::__TryInitCarry() : Failed attempt to Initialize Base_Interface using an invalid starting type, or attempt to construct interface incorrectly (use Base_Interface::Instantiate).");
				}
			}
		protected:		
			T_Implementor<T> impl;
			bool destruct_obj = false;
		public:
			// Public constructors should only be used for initially constructing an interface, since one cannot initialize a carry object with them from other methods. 
			Base_Interface() : destruct_obj(true), obj_ptr(new T), impl(T_Implementor<T>(*(obj_ptr), *this)), IfaceCarry<T_Carry>() { __TryInitCarry(); }
			Base_Interface(T& obj) : obj_ptr(&obj), impl(T_Implementor<T>(obj, *this)), IfaceCarry<T_Carry>() { __TryInitCarry(); }
                        Base_Interface(T& obj, T_Carry* _carry_obj) : obj_ptr(&obj), impl(T_Implementor<T>(obj, *this)), IfaceCarry<T_Carry>(*_carry_obj) {}
			~Base_Interface() { if(destruct_obj) { delete obj_ptr; } }

			T_Implementor<T>& Do() { return impl; }

			template<class retType>
			Base_Interface<retType, T_Implementor, T_Carry> Instantiate(retType& obj) { return Base_Interface<retType, T_Implementor, T_Carry>(obj, this->carry_obj); }  
		};
			
		template<class T, template<class> class T_Implementor>
		using Interface = Base_Interface<T, T_Implementor, RLE>;		
		
		template<class T> 
		class Specialized_Implementor;

		// These implementors are assuming RLE state is being carried.
		
		template<class T, template<class> class Ret_Implementor>
		class Default_Implementor{
		protected:
			T& obj;
			Interface<T, Specialized_Implementor>* iface;
		public:		     
			Default_Implementor(T& _obj, Interface<T, Specialized_Implementor>& _iface) : obj(_obj), iface(&_iface) {}
			Default_Implementor(T& _obj) : obj(_obj) {}
		      
			template<class getType, class argType = std::string>
			Ret_Implementor<getType>& Get(argType name) {
			 	throw std::runtime_error("Implementor::Get() : No such table with member type given with args std::string name");
			}
			template<class getType>
 			Ret_Implementor<getType>& Get(){
				throw std::runtime_error("Implementor::Get() : No such table with member type given with no args");	
			}
			template<class strType = std::string>
			Ret_Implementor<strType>& Name() {
				throw std::runtime_error("Implementor::Name() : Object with this interface does not have Name() defined for it"); 
			}
			template<typename setType>
			Ret_Implementor<T>& Set(std::string name, setType& rval) {
				throw std::runtime_error("Implementor::Set() : Object with this interface does not have Set() defined for it"); 				
			};
			template<typename checkType>
			Ret_Implementor<T>& Check(std::string name){
				throw std::runtime_error("Implementor::Check() : Object with this interface does not have Check() defined for it"); 					
			}
			T& GiveInstance() { return obj; }
			Interface<T, Specialized_Implementor>& GiveInterface() { return iface; }
			template<class retType>
			Interface<retType, Specialized_Implementor> Instantiate(retType& obj) { return iface->Instantiate(obj); }
			template<class retType>
			Interface<retType, Specialized_Implementor> Instantiate_ByVal(retType obj) { return iface->Instantiate(obj); } 
		};
		
		template<class U>
		class Specialized_Implementor : public Default_Implementor<U, Specialized_Implementor>{
		public:
			Specialized_Implementor(U& _obj, Interface<U, Specialized_Implementor>& _iface) : Default_Implementor<U, Specialized_Implementor>(_obj, _iface) {} 
		};;
	  
		// Specialization for tile::Layer
			
		template<>
		class Specialized_Implementor<tile::Layer> : public Default_Implementor<tile::Layer, Specialized_Implementor>{
		private:
			template<typename T_Derived>
			tile::Derived_Layer<T_Derived>& DownCastToDerived(){
				tile::Derived_Layer<T_Derived>* dl = dynamic_cast<tile::Derived_Layer<T_Derived>*>(this);
				if(dl){
					return *this; 
				}
				throw std::runtime_error("Implementor for tile::Layer : A dynamic cast failed from Layer -> Derived_Layer (Is the template argument the correct type for the layer?"); 
			}
		public:
			Specialized_Implementor<tile::Layer>(tile::Layer& _obj, Interface<tile::Layer, Specialized_Implementor>& _iface) : Default_Implementor<tile::Layer, Specialized_Implementor>(_obj, _iface) {}
			template<typename T>
			Specialized_Implementor<T> Data(unsigned int index){
				return Interface<T, interface::Specialized_Implementor>(DownCastToDerived<T>().GetData(index)).Do();
			}
			template<typename T>
			Specialized_Implementor<T> Data(unsigned int x, unsigned int y, unsigned int z){
				return Interface<T, interface::Specialized_Implementor>(DownCastToDerived<T>().GetData(x, y, z)).Do();
			}
			Specialized_Implementor<tile::Layer> AddLayer(unsigned int x, unsigned int y, unsigned int z); 
		};
			
		template<class T>
		using basic_interface = Interface<T, Specialized_Implementor>;
		using basic_irle = Interface<RLE, Specialized_Implementor>;						
	}
}

