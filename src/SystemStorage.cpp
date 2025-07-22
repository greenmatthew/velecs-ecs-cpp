/// @file    SystemStorage.cpp
/// @author  Matthew Green
/// @date    2025-07-22 14:49:18
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#include "velecs/ecs/SystemStorage.hpp"

#include "velecs/ecs/System.hpp"

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

SystemStorage& SystemStorage::operator=(SystemStorage&& other) noexcept
{
    if (this != &other)
    {
        system = std::move(other.system);
        typeIndex = other.typeIndex;
    }
    return *this;
}

// Public Methods

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

} // namespace velecs::ecs
