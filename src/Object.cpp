#include "velecs/ecs/Object.hpp"
// #include "velecs/ecs/World.hpp"

#include <memory>

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

// Public Methods

Object& Object::operator=(const Object& other) noexcept
{
    if (this != &other) {
        _world = other._world;
        _uuid = other._uuid;
        _name = other._name;
    }
    return *this;
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

} // namespace velecs::ecs
