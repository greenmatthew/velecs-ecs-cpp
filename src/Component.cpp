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

#include <stdexcept>

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

Component::~Component() = default;

// Public Methods

const Entity& Component::GetOwner() const
{
    if (!_ownerPtr)
    {
        throw std::runtime_error("Component has no owner entity");
    }

    return *_ownerPtr;
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

} // namespace velecs::ecs
