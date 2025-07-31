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

class Scene;

/// @class Component
/// @brief Abstract base class for all component types in the ECS system.
///
/// Components represent data and behavior that can be attached to entities.
/// All component types must inherit from this class to be used with the ECS.
/// Components should contain data relevant to a specific aspect of an entity
/// (e.g., Transform for position/rotation, MeshRenderer for visual representation).
class Component {
    friend class Entity;
    friend class Scene;

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

    Entity GetOwner() const;

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    Scene* _scene{nullptr};
    entt::entity _handle{entt::null};

    // Private Methods
};

} // namespace velecs::ecs
