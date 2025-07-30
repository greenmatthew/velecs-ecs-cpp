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

#include "velecs/ecs/TypeConstraints.hpp"

#include <entt/entt.hpp>

namespace velecs::ecs {

/// @class System
/// @brief Base class for all ECS systems that process entities and their components.
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

    // Lifecycle

    /// @brief Called once when the system is first registered with a scene.
    /// @details Override this method to perform one-time setup such as caching entity queries,
    ///          initializing resources, setting up event listeners, or configuring system-specific
    ///          data structures. This is called before any processing methods and guarantees
    ///          the system is ready for operation.
    ///          Default implementation does nothing.
    virtual void Init() {}

    /// @brief Called once when the system is removed from a scene or the scene is destroyed.
    /// @details Override this method to perform cleanup such as releasing resources,
    ///          unregistering event listeners, or saving persistent state. This ensures
    ///          proper resource management and prevents memory leaks.
    ///          Default implementation does nothing.
    virtual void Cleanup() {}

    // Main phases

    /// @brief Called every frame to update the system's main logic.
    /// @param deltaTime Time elapsed since the last frame in seconds.
    /// @details Override this method to implement the core system behavior that operates
    ///          on entities and their components. This is where most game logic happens,
    ///          including AI updates, animation, gameplay mechanics, and state management.
    ///          This phase runs before physics simulation to allow logic to influence
    ///          physical properties for the current frame.
    ///          Default implementation does nothing.
    virtual void Process(float deltaTime) {}

    /// @brief Called during the physics simulation phase.
    /// @details Override this method to implement physics-related logic such as applying forces,
    ///          handling collision responses, updating rigid body properties, or performing
    ///          physics queries. This phase runs after main logic processing but before
    ///          rendering to ensure visual representation reflects the current physics state.
    ///          Systems that don't interact with physics can leave this unimplemented.
    ///          Default implementation does nothing.
    virtual void ProcessPhysics() {}

    /// @brief Called during the GUI rendering and interaction phase.
    /// @details Override this method to implement user interface logic such as updating
    ///          UI element states, handling menu interactions, processing GUI input,
    ///          or managing HUD displays. This phase typically runs last to ensure
    ///          UI reflects the current game state after all logic and physics updates.
    ///          Systems without UI components can leave this unimplemented.
    ///          Default implementation does nothing.
    virtual void ProcessGUI() {}

    // System control

    /// @brief Gets the execution order priority for this system within each phase.
    /// @return Integer priority value where lower numbers execute first.
    /// @details Systems with the same priority execute in registration order.
    ///          Use negative values for systems that must run early (e.g., input handling),
    ///          positive values for systems that depend on others (e.g., rendering),
    ///          and 0 (default) for most gameplay systems.
    ///          Default implementation returns 0.
    virtual int GetExecutionOrder() const { return 0; }

    /// @brief Checks if this system is currently enabled for processing.
    /// @return true if the system will be processed during scene updates, false otherwise.
    /// @details Disabled systems skip all processing phases but remain registered with
    ///          the scene. This allows for temporary deactivation without the overhead
    ///          of removing and re-adding systems.
    inline bool IsEnabled() const { return _enabled; }

    /// @brief Enables or disables this system for processing.
    /// @param enabled true to enable processing, false to disable.
    /// @details Disabled systems will not have their Process, ProcessPhysics, or ProcessGUI
    ///          methods called during scene updates. Init and Cleanup are still called
    ///          during system registration/removal regardless of enabled state.
    ///          This provides a lightweight way to temporarily deactivate system behavior.
    inline void SetEnabled(bool enabled) { _enabled = enabled; }

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    bool _enabled{true}; ///< @brief Whether this system is currently enabled for processing.

    // Private Methods
};

} // namespace velecs::ecs
