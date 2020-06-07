#pragma once
#include <vector>
#include <string>
#include <variant>
#include <memory>
#include <tuple>

namespace rle{

	namespace entity { class Entity; } 
	
	namespace tile{		

		// Leave it at the interface level to downcast
	      
		
		class Layer {
		protected:
			std::string name;
			unsigned int max_x;
			unsigned int max_y;
			unsigned int max_z;
		public:
			Layer(std::string _name, unsigned int _max_x, unsigned int _max_y, unsigned int _max_z) : name(_name), max_x(_max_x), max_y(_max_y), max_z(_max_z) {}
			std::string Name() { return name; }
		};

		template<typename T>
		class Derived_Layer : protected Layer{
		protected:
			
			std::vector<T> data;
			T init_val;
		
		public:
			
			Derived_Layer(std::string _name, T _init_val, unsigned int _max_x, unsigned int _max_y, unsigned int _max_z) : Layer(_name, _max_x, _max_y, _max_z), init_val(_init_val) {
				data.reserve(max_x*max_y*max_z); 
				for(typename std::vector<T>::iterator i = data.begin(); i != data.end(); ++i){
					*i = init_val; 
				}
			}
			std::string Name() { return name; }
			T& GetData(unsigned int index){
				return *(data.at(index));
			}
			T& GetData(unsigned int x, unsigned int y, unsigned int z){
				return *(data.at(x + y*max_x + z*max_x*max_y));
			}
		
		};
		struct Tile{
			
			Tile(unsigned int _x, unsigned int _y, unsigned int _z) : x(_x), y(_y), z(_z) {} 
			
			std::vector<rle::entity::Entity*> entities;
			
			unsigned int x;
			unsigned int y;
			unsigned int z; 
			
			rle::entity::Entity& operator[](unsigned int index){
				return *(entities.at(index));  
			}
  			rle::entity::Entity& GetEntity(unsigned int index){
				return *(entities.at(index));
			}
		};
		
		class TileMap{
			
			std::string name;
			
			std::vector<Layer*> data_layers;
			std::vector<Tile> tiles;
			
			unsigned int size;
			
			unsigned int max_x;
			unsigned int max_y;
			unsigned int max_z;
		public:
			TileMap(std::string name, unsigned int x, unsigned int y, unsigned int z);
			~TileMap();
			std::string Name() { return name; }
			
			Tile& operator[](unsigned int i);
			Tile& operator() (unsigned int x, unsigned int y, unsigned int z);

			template<typename T>
			void NewLayer(std::string _name, T init_val);
			
			Layer& GetLayer(std::string _name);
		        Layer& GetLayer(unsigned int index);				      
			
			void DelLayer(std::string _name);
			void DelLayer(unsigned int index);
			
			std::tuple<unsigned int, unsigned int, unsigned int> getSizeTuple() { return {max_x, max_y, max_z}; }
			
			unsigned int getMax_x() { return max_x; }
			unsigned int getMax_y() { return max_y; }
			unsigned int getMax_z() { return max_z; }

			unsigned int getSize() { return size; }

		};	
	}  
}
