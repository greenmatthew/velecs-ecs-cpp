#include "velecs/ecs/World.hpp"

namespace velecs::ecs {

// Public Methods

template<typename ObjectT, typename... Args>
ObjectT* Object::Create(World* const world, Args&&... args)
{
    return world->template Create<ObjectT>(world, std::forward<Args>(args)...);
}

template<typename StorageT, typename ObjectT, typename... Args>
ObjectT* Object::CreateAs(World* const world, Args&&... args)
{
    return world->template CreateAs<StorageT, ObjectT>(world, std::forward<Args>(args)...);
}

// Protected Methods

// Private Methods

} // namespace velecs::ecs
