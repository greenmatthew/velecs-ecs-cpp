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
#include "velecs/ecs/EntityRegistry.hpp"

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

Entity::Entity(entt::registry& registry, entt::entity handle)
    : registry(registry), handle(handle), name("Entity") {}

// Public Methods

Entity Entity::Create()
{
    entt::registry& registry = EntityRegistry::GetRegistry();
    entt::entity handle =  registry.create();
    Entity entity = Entity(registry, handle);
    return entity;
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

} // namespace velecs::ecs
