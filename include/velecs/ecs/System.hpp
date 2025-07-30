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

class Scene;

/// @class System
/// @brief Base class for all ECS systems that process entities and their components.
class System {
    friend class Scene; // Allow Scene to call protected lifecycle methods

public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor.
    System() = default;

    /// @brief Pure virtual destructor to ensure proper cleanup of derived classes.
    virtual ~System() = 0;

    // Public Methods

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

    /// @brief Safely casts a void* context to a specific context type.
    /// @tparam ContextType The target context type to cast to.
    /// @param context The void* context pointer passed from the scene processing methods.
    /// @return A pointer to the context cast to the specified type.
    /// @details This helper method provides type-safe access to engine-specific context data
    ///          within system processing methods. The context type is determined by the engine
    ///          and can vary between different processing phases (e.g., PhysicsContext for
    ///          ProcessPhysics, RenderContext for ProcessGUI).
    /// @note It is the caller's responsibility to ensure the void* context actually points
    ///       to an object of ContextType. Incorrect casting will result in undefined behavior.
    /// 
    /// @code
    /// void ProcessPhysics(void* context) override {
    ///     auto* physicsCtx = Context<PhysicsContext>(context);
    ///     float deltaTime = physicsCtx->GetDeltaTime();
    ///     // ... use physics-specific context data
    /// }
    /// @endcode
    template<typename ContextType>
    ContextType* Context(void* context) const
    {
        return static_cast<ContextType*>(context);
    }

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

    /// @brief Called during the main logic processing phase for each frame.
    /// @param context Execution context data passed from the scene (currently void* placeholder).
    /// @details Override this method to implement the core system behavior such as updating
    ///          component values, processing game logic, or handling entity state changes.
    ///          This is the primary update phase where most gameplay systems should execute.
    ///          Only called if the system is enabled.
    ///          Default implementation does nothing.
    virtual void Process(void* context) {}

    /// @brief Called during the physics simulation phase for each frame.
    /// @param context Execution context data passed from the scene (currently void* placeholder).
    /// @details Override this method to implement physics-related updates such as applying
    ///          forces, handling collisions, or updating physics bodies. This phase typically
    ///          runs after the main logic phase to ensure proper separation of concerns.
    ///          Only called if the system is enabled.
    ///          Default implementation does nothing.
    virtual void ProcessPhysics(void* context) {}

    /// @brief Called during the GUI rendering phase for each frame.
    /// @param context Execution context data passed from the scene (currently void* placeholder).
    /// @details Override this method to implement GUI-related updates such as updating UI
    ///          elements, handling menu interactions, or drawing debug information. This phase
    ///          typically runs last to ensure GUI elements appear on top of game content.
    ///          Only called if the system is enabled.
    ///          Default implementation does nothing.
    virtual void ProcessGUI(void* context) {}

private:
    // Private Fields

    bool _enabled{true}; ///< @brief Whether this system is currently enabled for processing.

    // Private Methods
};

} // namespace velecs::ecs
