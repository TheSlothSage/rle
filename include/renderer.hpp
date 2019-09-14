#include <string> 
#include "entity.hpp"
#include "component.hpp"

/*
 * This class will operate as an abstract renderer, which takes a tilemap and decides what should be displayed at what tile, or gets an array
 * of entity object pointers covering the whole rendering space (depending on window size and whatnot).
 * 
 * This allows for renderers to be built on top to do the actual GPU "rendering" on update.
 *
 * The visitor design pattern is used to simplify the process of rendering. This also allows for a type safe way to manager differnt types of 
 * renderers without worrying too much about implementation per-se. Adding a new engine is as easy as specializing Element<thisType>::accept, and * specializing the Visitor<elementType>::visit as well. 
 *
 * 		- The Sloth Sage
*/ 


namespace rle{
	namespace render{		
		
		namespace visitor{
			
			// Specific renderers will use this visitor design pattern to implement a rendering engine. 	
			
			template<class elementType>
			struct Visitor{
				void visit(elementType* e) {}   		
			};
			
		}
		
		template <class thisType>
		class Element {
		
			protected:

				std::vector<rle::entity::Entity*> entity_buf; 
				const unsigned int x;
				const unsigned int y;
				
				void accept(visitor::Visitor<thisType>* v) { v->visit(this); }

			public:
			
				// waste no time in filling the vector with nullptrs. 

				Element(unsigned int x_, unsigned int y_) : x(x_), y(y_) 
				{ entity_buf.reserve(x_*y_); std::fill(entity_buf.begin(), entity_buf.end(), nullptr); } 
				
				~Element() = default;

				unsigned int getX() { return x; }
				unsigned int getY() { return y; } 

		};

	}
}


