#include "velecs/ecs/World.hpp"

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

// Public Methods

size_t World::GetTotalCount() const
{
    size_t total = 0;
    for (const auto& [type, objMap] : _objects)
    {
        total += objMap.size();
    }
    return total;
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

} // namespace velecs::ecs
