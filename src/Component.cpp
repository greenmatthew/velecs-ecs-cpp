/// @file    Component.cpp
/// @author  Matthew Green
/// @date    2025-05-12 16:28:03
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#include "velecs/ecs/Component.hpp"
#include "velecs/ecs/Entity.hpp"

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

// Public Methods

Entity Component::GetOwner() const
{
    return owner ? *owner : Entity::INVALID;
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

void Component::SetOwner(const Entity& entity)
{
    owner = const_cast<Entity*>(&entity);
}

} // namespace velecs::ecs
