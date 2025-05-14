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
    Component() = default;

    /// @brief Default deconstructor.
    ~Component() = default;

    // Public Methods

    Entity GetOwner() const;

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    Entity* owner = nullptr;

    // Private Methods

    void SetOwner(const Entity& entity);
};

} // namespace velecs::ecs
