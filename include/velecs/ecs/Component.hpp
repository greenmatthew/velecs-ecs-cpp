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

#include <memory>

namespace velecs::ecs {

class Entity;

/// @class Component
/// @brief Brief description.
///
/// Rest of description.
class Component {
    friend class Entity;

public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor.
    Component();

    /// @brief Copy constructor.
    /// @param other The component to copy from.
    Component(const Component& other);

    /// @brief Default deconstructor.
    ~Component() = default;

    // Public Methods

    /// @brief Copy assignment operator.
    /// @param other The component to copy from.
    /// @return Reference to this component after assignment.
    Component& operator=(const Component& other);
    
    /// @brief Move constructor.
    /// @param other The component to move from.
    Component(Component&& other) noexcept;
    
    /// @brief Move assignment operator.
    /// @param other The component to move from.
    /// @return Reference to this component after assignment.
    Component& operator=(Component&& other) noexcept;

    /// @brief Gets the owning entity of this component.
    /// @return The entity that owns this component.
    Entity GetOwner() const;

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    std::unique_ptr<Entity> ownerPtr;

    // Private Methods

    void SetOwner(const Entity& entity);
};

} // namespace velecs::ecs
