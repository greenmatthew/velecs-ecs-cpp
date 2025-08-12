/// @file    ObjectManager.cpp
/// @author  Matthew Green
/// @date    2025-08-12 11:43:11
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#include "velecs/ecs/ObjectManager.hpp"

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

// Public Methods

size_t ObjectManager::GetTotalCount() const
{
    size_t total = 0;
    for (const auto& [type, objMap] : _objects)
    {
        total += objMap.size();
    }
    return total;
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

} // namespace velecs::ecs
