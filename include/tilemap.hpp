#pragma once
#include <vector>
#include <string>
#include <variant>
#include <memory>
#include <experimental/any> 

namespace rle{

	namespace entity { class Entity; } 
	
	namespace tile{		
		
		class LayerIface{
			std::string name;
		private:
			virtual void a_Put(std::experimental::any _rval) = 0;
			virtual void a_Set(std::experimental::any _lval) = 0;			
		public:
			LayerIface(std::string _name) : name(_name) {}		
			std::string Name(){
				return name;
			}		       			
			virtual LayerIface& Get(unsigned int index) = 0;
			
			template<typename T>
			void Put(T rval);
			template<typename T>
			void Set(T rval); 
		};
		
		template<typename T> class Layer : LayerIface{
		private:
			unsigned int size;
			std::vector<T> data;
			T init_val;
			T& get_ret;
		       
			void a_Put(std::experimental::any _rval);
			void a_Set(std::experimental::any _lval);

		public:
			Layer(std::string _name, T _init_val, unsigned int _size) : LayerIface(_name),
										    init_val(_init_val), size(_size) {
				data.reserve(size); 
				for(typename std::vector<T>::iterator i = data.begin(); i != data.end(); ++i){
					*i = init_val; 
				}
			}					       
			LayerIface& Get(unsigned int index);			
		};
		
		struct Tile{
			Tile(unsigned int _x, unsigned int _y, unsigned int _z) : x(_x), y(_y), z(_z) {} 
			std::vector<rle::entity::Entity*> entities;
			unsigned int x;
			unsigned int y;
			unsigned int z; 
			rle::entity::Entity* operator[](unsigned int index){
				return entities.at(index);  
			}
			rle::entity::Entity* GetEntity(unsigned int index){
				return entities.at(index);
			}
		};
		
		class TileMap{
			std::string name;
			std::vector<LayerIface> data_layers;
			std::vector<Tile> tiles;
			unsigned int size;
			unsigned int max_x;
			unsigned int max_y;
			unsigned int max_z;
		public:
			TileMap(std::string name, unsigned int x, unsigned int y, unsigned int z);

			std::string Name() { return name; }
			
			Tile& operator[](unsigned int i);
			Tile& operator() (unsigned int x, unsigned int y, unsigned int z);

			template<typename T>
			void NewLayer(std::string _name, T init_val);
			LayerIface& GetLayer(std::string _name);
		        LayerIface& GetLayer(unsigned int index);				      
			void DelLayer(std::string _name);
			void DelLayer(unsigned int index); 
		};	
	}  
}
