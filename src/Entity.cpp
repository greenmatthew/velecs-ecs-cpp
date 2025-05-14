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
#include "velecs/ecs/components/Name.hpp"
#include "velecs/ecs/components/Transform.hpp"

#include <memory>

namespace velecs::ecs {

// Public Fields

const Entity Entity::INVALID;

// Constructors and Destructors

// Public Methods

Entity Entity::Create()
{
    entt::entity handle = registry.create();
    Entity result = Entity(handle);
    result.AddComponent<Name>("Entity");
    result.AddComponent<Transform>();
    return result;
}

std::string Entity::GetName() const
{
    return GetComponent<Name>().GetName();
}

void Entity::SetName(const std::string& newName)
{
    GetComponent<Name>().SetName(newName);
}

Transform& Entity::GetTransform() const
{
    return GetComponent<Transform>();
}

void Entity::RequestDestroy(Entity entity)
{
    if (entity.IsValid())
    {
        destructionQueue.push_back(entity);
    }
}

void Entity::ProcessDestructionQueue()
{
    for (auto& entity : destructionQueue)
    {
        entity.Destroy();
    }
    
    destructionQueue.clear();
}

// Protected Fields

// Protected Methods

// Private Fields

entt::registry Entity::registry;
std::vector<Entity> Entity::destructionQueue;

// Private Methods

void Entity::Destroy()
{
    registry.destroy(handle);
}

} // namespace velecs::ecs
