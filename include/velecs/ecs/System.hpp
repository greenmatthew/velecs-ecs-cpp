/// @file    System.hpp
/// @author  Matthew Green
/// @date    2025-07-22 11:08:28
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include "velecs/ecs/Registry.hpp"
#include "velecs/ecs/TypeConstraints.hpp"
#include "velecs/ecs/SystemStorage.hpp"

#include <entt/entt.hpp>

namespace velecs::ecs {

/// @class System
/// @brief Brief description.
///
/// Rest of description.
class System {
public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor.
    System() = default;

    /// @brief Pure virtual destructor to ensure proper cleanup of derived classes.
    virtual ~System() = 0;

    // Public Methods

    inline static entt::registry& GetRegistry() { return Registry::Get(); }

    /// @brief Called once when the system is first created.
    /// @details Override this method to perform one-time setup such as caching queries,
    ///          initializing resources, or setting up system-specific data structures.
    ///          Default implementation does nothing.
    virtual void Init() {}

    /// @brief Called every frame to update the system's logic.
    /// @param deltaTime Time elapsed since the last frame in seconds.
    /// @details Override this method to implement the core system behavior that operates
    ///          on entities and their components. This is where the main ECS logic happens.
    ///          Default implementation does nothing.
    virtual void Update(float deltaTime) {}

    /// @brief Attempts to add a new system of the specified type to the registry.
    /// @tparam SystemType The type of system to add. Must inherit from System.
    /// @return true if the system was successfully added, false if a system of this type already exists.
    template<typename SystemType, typename = IsSystem<SystemType>>
    static bool TryAddSystem()
    {
        auto& registry = GetRegistry();
        std::type_index targetType = std::type_index(typeid(SystemType));
        
        // Check if system of this type already exists
        auto view = registry.view<SystemStorage>();
        for (auto entity : view)
        {
            const auto& storage = view.get<SystemStorage>(entity);
            if (storage.typeIndex == targetType) return false; // System already exists
        }

        // Create new system instance
        auto systemInstance = std::make_unique<SystemType>();
        
        // Initialize the system
        systemInstance->Init();
        
        // Create entity and storage
        auto entity = registry.create();
        registry.emplace<SystemStorage>(entity, std::move(systemInstance), targetType);
        
        return true;
    }

    /// @brief Attempts to remove a system of the specified type from the registry.
    /// @tparam SystemType The type of system to remove. Must inherit from System.
    /// @return true if the system was found and removed, false if no system of this type exists.
    template<typename SystemType, typename = IsSystem<SystemType>>
    static bool TryRemoveSystem()
    {
        auto& registry = GetRegistry();
        std::type_index targetType = std::type_index(typeid(SystemType));
        
        // Find system of this type
        auto view = registry.view<SystemStorage>();
        for (auto entity : view)
        {
            const auto& storage = view.get<SystemStorage>(entity);
            if (storage.typeIndex == targetType)
            {
                registry.destroy(entity);
                return true;
            }
        }
        
        return false; // System not found
    }

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    // Private Methods
};

} // namespace velecs::ecs
