/// @file    Entity.cpp
/// @author  Matthew Green
/// @date    2025-05-12 16:24:39
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#include "velecs/ecs/Entity.hpp"
#include "velecs/ecs/EntityBuilder.hpp"
#include "velecs/ecs/Scene.hpp"

#include "velecs/ecs/tags/DestroyTag.hpp"

#include "velecs/ecs/components/Name.hpp"
#include "velecs/ecs/components/Transform.hpp"

#include <memory>
#include <string>
#include <algorithm>

namespace velecs::ecs {

// Public Fields

const Entity Entity::INVALID;

// Constructors and Destructors

Entity::Entity(Scene* const scene, const entt::entity handle)
    : _scene(scene), _handle(handle) {}

EntityBuilder Entity::Create(Scene* const scene)
{
    return scene->CreateEntity();
}

// Public Methods

bool Entity::IsValid() const
{
    return _scene != nullptr
        && _scene->IsEntityValid(*this);
}

const std::string& Entity::GetName() const
{
    const Name* name{nullptr}; // Use const pointer since this is a const method
    TryGetComponent<Name>(name);
    assert(name != nullptr && "Entity missing required Name component. All entities must have Name and Transform components.");
    return name->GetName();
}

void Entity::SetName(const std::string& newName)
{
    Name* name{nullptr};
    TryGetComponent<Name>(name);
    assert(name != nullptr && "Entity missing required Name component. All entities must have Name and Transform components.");
    name->SetName(newName);
}

Transform& Entity::GetTransform() const
{
    const Transform* transform{nullptr}; // Use const pointer since this is a const method
    TryGetComponent<Transform>(transform);
    assert(transform != nullptr && "Entity missing required Transform component. All entities must have Name and Transform components.");
    return const_cast<Transform&>(*transform); // Safe cast since Transform is mutable conceptually
}

void Entity::RequestDestroy(Entity entity)
{
    if (entity.IsValid()) entity.AddTag<DestroyTag>();
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

} // namespace velecs::ecs
