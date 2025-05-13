/// @file    EntityRegistry.hpp
/// @author  Matthew Green
/// @date    2025-05-12 16:49:45
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include <entt/entt.hpp>

namespace velecs::ecs {

/// @class EntityRegistry
/// @brief Brief description.
///
/// Rest of description.
class EntityRegistry {
public:
    // Enums

    // Public Fields

    static EntityRegistry& Get() {
        static EntityRegistry instance;
        return instance;
    }

    static entt::registry& GetRegistry()
    {
        return Get().registry;
    }

    // Constructors and Destructors

    /// @brief Delete copy constructor
    EntityRegistry(const EntityRegistry&) = delete;
    
    /// @brief Delete assignment operator
    EntityRegistry& operator=(const EntityRegistry&) = delete;

    /// @brief Default deconstructor.
    ~EntityRegistry() = default;

    // Public Methods

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    entt::registry registry;

    // Private Methods

    /// @brief Default constructor.
    EntityRegistry() = default;
};

} // namespace velecs::ecs
