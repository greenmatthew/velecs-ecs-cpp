#include "velecs/ecs/Component.hpp"
#include "velecs/ecs/Entity.hpp"
#include "velecs/ecs/Components/Transform.hpp"

#include <stdexcept>

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

Component::~Component() = default;

// Public Methods

Entity* Component::GetOwner() const
{
    return _owner;
}

Scene* Component::GetScene() const
{
    return GetOwner()->GetScene();
}

Transform& Component::GetTransform() const
{
    return GetOwner()->GetTransform();
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

} // namespace velecs::ecs
