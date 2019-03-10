#include "irle.hpp"

template<>
rle::interface::Interface<rle::entity::Entity, rle::interface::Implementor<rle::entity::Entity>> rle::interface::Implementor<rle::RLE>::Get<rle::entity::Entity>(std::string name){
	rbuf = std::experimental::any(obj.GetEntity(name));
}

