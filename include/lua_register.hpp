#pragma once 

extern "C"{ 
#include "math.h" 
}

#include <tuple>

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
	
	namespace entity { struct lua_Entity; } 

	namespace tile{

		struct lua_Tile : public lua_Reg<rle::tile::Tile>{
			
			lua_Tile(rle::interface::basic_irle* rle, rle::tile::Tile* tile) : lua_Reg(rle, tile) {} 
			
			rle::entity::lua_Entity getEntityByName(std::string name);
			entity::lua_Entity getEntity(unsigned int index);  
			
			int entityCount() const { return container->entities.size(); }
		};

		struct lua_TileMap : public lua_Reg<rle::tile::TileMap>{
			lua_TileMap(rle::interface::basic_irle* rle, rle::tile::TileMap* tilemap) : lua_Reg(rle, tilemap) {} 
			~lua_TileMap() = default; 
			
			std::string Name() const { return container->Name(); }

			lua_Number mapSize() const  { return container->getSize(); }

			int mapX() const { return container->getMax_x(); }
			int mapY() const { return container->getMax_y(); }		
			int mapZ() const { return container->getMax_z(); }

			lua_Tile tile(unsigned int x, unsigned int y) { return lua_Tile(rle, &((*container)[x,y])); }     
		};
	}
		    
        namespace entity {
        	struct lua_Entity : protected rle::lua_Reg<rle::entity::Entity> {
                	
			lua_Entity(rle::interface::basic_irle* rle, Entity* entity) : lua_Reg(rle, entity) {}
                	~lua_Entity() = default;        
    
      	        	int getTile_X() const { return container->X(); }
		       	int getTile_Y() const { return container->Y(); }
	 		int getTile_Z() const { return container->Z(); }
    
			void setTile_X(double x) { container->setX(::round(x)); }
			void setTile_Y(double y) { container->setY(::round(y)); }
	   		void setTile_Z(double z) { container->setZ(::round(z)); }
		   
			component::lua_Component get(std::string key) {
			       	return component::lua_Component(rle, &(container->GetComponent(key)));
			}
			std::string name() const {
			    	return container->Name();
			}
			lua_Entity& addComponent(std::string component) {
				recursive_ResolveDependencies(component, rle->Do().LuaState(), &container->component_table);
				// container->AddComponent(new component::Component(component, rle->Do().LuaState()));
				return *this;
			}
			lua_Entity& delComponent(std::string component) {
				rle->Instantiate<rle::entity::Entity>(*container).Do().delComponent(component);
				return *this;
			}
			// for tilemap property 
			rle::tile::lua_TileMap getTileMap() const {
			     	return rle::tile::lua_TileMap(rle, &(container->getTileMap()));
			}
			void setTileMap(std::string name){
				container->setTileMap(rle->Do().getTileMap(name));
		       	}
		       	rle::tile::lua_Tile getTile() const {
				return rle::tile::lua_Tile(rle, &(container->Tile()));     
		       	}
			void setTile(unsigned int x, unsigned int y, unsigned int z) const {
				container->setX(x);
				container->setY(y);
				container->setZ(z);
				container->UpdateTilePtr();
			}
	       	};
       	}		
}	
