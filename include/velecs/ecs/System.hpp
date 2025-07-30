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

    virtual void Process(void* context) {}
    virtual void ProcessPhysics(void* context) {}
    virtual void ProcessGUI(void* context) {}

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
