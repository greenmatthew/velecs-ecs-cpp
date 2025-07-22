/// @file    Component.hpp
/// @author  Matthew Green
/// @date    2025-05-12 16:27:30
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include "velecs/ecs/TypeConstraints.hpp"

#include <memory>

#include <entt/entt.hpp>

namespace velecs::ecs {

class Entity;

/// @class Component
/// @brief Abstract base class for all component types in the ECS system.
///
/// Components represent data and behavior that can be attached to entities.
/// All component types must inherit from this class to be used with the ECS.
/// Components should contain data relevant to a specific aspect of an entity
/// (e.g., Transform for position/rotation, MeshRenderer for visual representation).
class Component {
    friend class Entity;

public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor.
    Component() = default;

    /// @brief Pure virtual destructor to make class abstract.
    /// @details Ensures proper cleanup of derived component types and makes
    ///          this class abstract so it cannot be instantiated directly.
    virtual ~Component() = 0;

    // Public Methods

    /// @brief Gets the entity that owns this component instance.
    /// @tparam T The concrete component type (must be specified explicitly)
    /// @return The Entity that contains this component
    /// @warning Template parameter deduction does not work automatically - you must
    ///          specify the concrete component type explicitly when calling this method.
    ///
    /// @code
    /// class Camera : public Component {
    /// public:
    ///     void SomeMethod() const {
    ///         // Must specify the concrete type explicitly:
    ///         auto entity = GetOwner<Camera>();
    ///         
    ///         // This will NOT work (template deduction fails):
    ///         // auto entity = GetOwner(); // ERROR!
    ///     }
    /// };
    /// @endcode
    template<typename T, typename = IsComponent<T>>
    Entity GetOwner() const
    {
        return GetOwner<T>(*(T*)this);
    }

    /// @brief Static method to get the entity that owns a specific component instance.
    /// @tparam T The concrete component type
    /// @param comp Reference to the component instance
    /// @return The Entity that contains the given component
    /// @details This is the underlying implementation used by the instance method.
    ///          Uses EnTT's to_entity function to efficiently find the owning entity.
    template<typename T, typename = IsComponent<T>>
    static Entity GetOwner(const T& comp)
    {
        auto& registry = Entity::GetRegistry();
        const entt::entity entity = entt::to_entity(registry.storage<T>(), comp);
        return Entity(entity);
    }

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    // Private Methods
};

} // namespace velecs::ecs
