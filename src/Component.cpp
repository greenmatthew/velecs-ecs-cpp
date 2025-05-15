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

Component::Component()
    : ownerPtr(std::make_unique<Entity>(Entity::INVALID)) {}

Component::Component(const Component& other) 
    : ownerPtr(std::make_unique<Entity>(*other.ownerPtr)) {}

// Public Methods

Component& Component::operator=(const Component& other) 
{
    if (this != &other)
    {
        if (!ownerPtr)
        {
            ownerPtr = std::make_unique<Entity>(*other.ownerPtr);
        }
        *ownerPtr = *other.ownerPtr;
    }
    return *this;
}

Entity Component::GetOwner() const
{
    return *ownerPtr;
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

void Component::SetOwner(const Entity& entity)
{
    ownerPtr = std::make_unique<Entity>(entity);
}

} // namespace velecs::ecs
