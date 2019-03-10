#pragma once

#include "rle.hpp"
#include <experimental/any>

namespace rle{
	namespace interface{
		template<class T, class T_Implementor<typename TL>>
		class Interface{
		protected:		
			T_Implementor& impl;			
		public:
			Interface() : impl(*(new T_Implementor<T>(*(new T)))) {}
			Interface(T& obj) : impl(*(new T_Implementor(obj))) {}
			template<class P, class P_Implementor>
			Interface<P, P_Implementor>& Switch(std::string name);
			template<class P, class P_Implementor>
			Interface<P, P_Implementor>& Switch(unsigned int index);
			Interface<T, T_Implementor>& Put(std::experimental::any& rval);
			Interface<T, T_Implementor>& Set(std::experimental::any& lval);
		};

		template<typename T>
		class Implementor{
		protected:			
			std::experimental::any rbuf;
			T& obj;
		public:
			Implementor() : obj(*(new T())) {}
			Implementor(T& _obj) : obj(_obj) {}
			template<typename getType>
			Interface<getType, Implementor<getType>> Get(std::string name) {}
			template<typename getType>
			Interface<getType, Implementor<getType>> Get(unsigned int index) {}
		};
		template<>
		class Implementor<rle::RLE>{
		public:
			
		};
	}
}
