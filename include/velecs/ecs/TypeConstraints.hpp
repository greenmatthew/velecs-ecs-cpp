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
class System;

template <typename T>
using IsEntity = std::enable_if_t<std::is_base_of_v<Entity, T>>;

/// @brief Validates tag types with custom error messages
template<typename T>
constexpr void validate_tag() {
    static_assert(std::is_base_of_v<Tag, T>, 
        "Type must inherit from Tag. Use Component for types with data members.");
    
    static_assert(sizeof(T) == sizeof(Tag), 
        "Tag must not contain any data members beyond the base Tag class. "
        "If you need to store data, inherit from Component instead.");
    
    static_assert(!std::is_same_v<T, Tag>, 
        "Cannot use Tag base class directly. Create a specific tag type.");
}

/// @brief Tag constraint that ensures tags are empty AND inherit from Tag
template <typename T>
using IsTag = std::enable_if_t<(validate_tag<T>(), std::is_base_of_v<Tag, T>)>;

/// @brief Validates component types with custom error messages
template<typename T>
constexpr void validate_component() {
    static_assert(std::is_base_of_v<Component, T>, 
        "Type must inherit from Component. Use Tag for marker types without data.");
    
    static_assert(!std::is_empty_v<T>, 
        "Components cannot be empty. Empty types should inherit from Tag instead.");
    
    static_assert(sizeof(T) > sizeof(Component), 
        "Component must contain at least one data member beyond the base Component class.");
    
    static_assert(!std::is_same_v<T, Component>, 
        "Cannot use Component base class directly. Create a specific component type.");
    
    static_assert(!std::is_abstract_v<T>, 
        "Cannot use abstract component types. Use a concrete derived class instead.");
}

/// @brief Component constraint that ensures components have data AND inherit from Component
template <typename T>
using IsComponent = std::enable_if_t<(validate_component<T>(), std::is_base_of_v<Component, T>)>;

/// @brief Validates system types with custom error messages
template<typename T>
constexpr void validate_system() {
    static_assert(std::is_base_of_v<System, T>, 
        "Type must inherit from System.");
    
    static_assert(!std::is_same_v<T, System>, 
        "Cannot use System base class directly. Create a specific system type.");
    
    static_assert(!std::is_abstract_v<T>, 
        "Cannot use abstract system types. System must implement Update() method.");
}

/// @brief System constraint that ensures systems are concrete subclasses of System
template <typename T>
using IsSystem = std::enable_if_t<(validate_system<T>(), std::is_base_of_v<System, T>)>;

} // namespace velecs::ecs
