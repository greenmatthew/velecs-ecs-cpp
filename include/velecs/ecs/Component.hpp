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
    virtual ~Component() = 0;

    // Public Methods

    template<typename T, typename = IsComponent<T>>
    Entity GetOwner() const
    {
        return GetOwner<T>(*(T*)this);
    }

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
