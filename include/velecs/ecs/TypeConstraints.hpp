/// @file    TypeConstraints.hpp
/// @author  Matthew Green
/// @date    2025-05-12 17:32:24
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include <type_traits>

namespace velecs::ecs {

class Entity;
class Tag;
class Component;

template <typename T>
using IsEntity = std::enable_if_t<std::is_base_of_v<Entity, T>>;

template <typename T>
using IsTag = std::enable_if_t<std::is_base_of_v<Tag, T>>;

/// @brief Helper function to validate component types with custom error messages
template<typename T>
constexpr bool is_valid_component() {
    static_assert(std::is_base_of_v<Component, T>, 
        "Type must inherit from Component. Use Tag for marker types without data.");
    
    static_assert(!std::is_empty_v<T>, 
        "Components cannot be empty. Empty types should inherit from Tag instead.");
    
    static_assert(sizeof(T) > sizeof(Component), 
        "Component must contain at least one data member beyond the base Component class.");
    
    static_assert(!std::is_same_v<T, Component>, 
        "Cannot use Component base class directly. Create a specific component type.");
    
    return true;
}

/// @brief Enhanced component constraint that ensures components have data AND inherit from Component
template <typename T>
using IsComponent = std::enable_if_t<is_valid_component<T>()>;

} // namespace velecs::ecs