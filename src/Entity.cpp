#include "velecs/ecs/Entity.hpp"
#include "velecs/ecs/EntityBuilder.hpp"
#include "velecs/ecs/Scene.hpp"

#include "velecs/ecs/tags/DestroyTag.hpp"

#include "velecs/ecs/components/Transform.hpp"

#include <memory>
#include <string>
#include <algorithm>

namespace velecs::ecs {

// Public Fields

const Entity Entity::INVALID;

// Constructors and Destructors

EntityBuilder Entity::Create(Scene* const scene)
{
    return scene->CreateEntity();
}

// Public Methods

bool Entity::IsValid() const
{
    return _handle != entt::null
        && GetWorld() != nullptr
        && _scene != nullptr
        && _scene->IsEntityHandleValid(this);
}

void Entity::MarkForDestruction()
{
    if (IsValid()) TryAddTag<DestroyTag>();
}

Transform& Entity::GetTransform() const
{
    const Transform* transform{nullptr}; // Use const pointer since this is a const method
    TryGetComponent<Transform>(transform);
    assert(transform != nullptr && "Entity missing required Transform component. All entities must have Name and Transform components.");
    return const_cast<Transform&>(*transform); // Safe cast since Transform is mutable conceptually
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

} // namespace velecs::ecs
