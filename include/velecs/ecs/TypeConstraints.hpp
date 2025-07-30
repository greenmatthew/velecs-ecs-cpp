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

class Scene;
class Entity;
class Tag;
class Component;
class System;



// ========== Scene Type Validation ==========



/// @brief Validates scene types with custom error messages
template<typename T>
constexpr void validate_scene() {
    static_assert(std::is_base_of_v<Scene, T>, 
        "Type must inherit from Scene.");
    
    static_assert(!std::is_same_v<T, Scene>, 
        "Cannot use Scene base class directly. Create a specific scene type.");
    
    static_assert(!std::is_abstract_v<T>, 
        "Cannot use abstract scene types. Scene must implement required methods.");
}

/// @brief Scene constraint that ensures scenes are concrete subclasses of Scene
template <typename T>
using IsScene = std::enable_if_t<(validate_scene<T>(), std::is_base_of_v<Scene, T>)>;



// ========== Entity Type Validation ==========



template <typename T>
using IsEntity = std::enable_if_t<std::is_base_of_v<Entity, T>>;



// ========== Tag Type Validation ==========



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



// ========== Component Type Validation ==========



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



// ========== Tag OR Component Type Validation ==========




/// @brief Validates tag or component types with custom error messages
template<typename T>
constexpr void validate_tag_or_component() {
    if constexpr (std::is_base_of_v<Tag, T>) {
        validate_tag<T>();
    } else if constexpr (std::is_base_of_v<Component, T>) {
        validate_component<T>();
    } else {
        static_assert(std::is_base_of_v<Tag, T> || std::is_base_of_v<Component, T>, 
            "Type must inherit from either Tag or Component for use in ECS queries.");
    }
}

/// @brief Tag or Component constraint that validates the type appropriately
template <typename T>
using IsTagOrComponent = std::enable_if_t<(validate_tag_or_component<T>(), 
    std::is_base_of_v<Tag, T> || std::is_base_of_v<Component, T>)>;



// ========== System Type Validation ==========



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
