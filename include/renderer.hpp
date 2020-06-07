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

				unsigned int x;
				unsigned int y;

				void accept(visitor::Visitor<thisType>* v) { v->visit(this); }

			public:
			
				// waste no time in filling the vector with nullptrs. 

				Element(unsigned int x_, unsigned int y_) 
				        : x(x_), y(y_) {} 
				
				~Element() = default;
                                
                                rle::entity::Entity* getEntity(unsigned int x, unsigned int y, unsigned int z){
                                        
                                } 
				
				unsigned int getX() const { return x; }
				unsigned int getY() const { return y; } 

				void setX(unsigned int x_) { x = x_; }
				void setY(unsigned int y_) { y = y_; } 
		};

	
	}
}


