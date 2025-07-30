/// @file    SceneManager.hpp
/// @author  Matthew Green
/// @date    2025-07-27 19:44:13
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include "velecs/ecs/TypeConstraints.hpp"

#include <velecs/common/Uuid.hpp>
using velecs::common::Uuid;
#include <velecs/common/NameUuidRegistry.hpp>
using velecs::common::NameUuidRegistry;

#include <string>
#include <memory>

namespace velecs::ecs {

class Scene;

/// @class SceneManager
/// @brief Manages scene lifecycle and handles transitions between different game scenes.
///
/// The SceneManager provides centralized control over scene registration, activation, and cleanup.
/// It maintains a registry of all available scenes and tracks the currently active scene.
/// Scene transitions are handled safely with proper OnExit/OnEnter lifecycle calls.
///
/// Scenes are stored using unique_ptr for clear ownership semantics, while the active scene
/// is tracked via a raw pointer for performance. This is safe because the SceneManager
/// controls both the registry and the active scene pointer.
///
/// @note All scene operations return boolean values to indicate success/failure rather than
///       throwing exceptions, allowing for graceful error handling in game loops.
///
/// @code
/// SceneManager sceneManager;
/// 
/// // Register scenes
/// auto menuUuid = sceneManager.RegisterScene<MainMenuScene>("MainMenu");
/// auto gameUuid = sceneManager.RegisterScene<GameplayScene>("Level1");
/// 
/// // Transition between scenes
/// if (sceneManager.TryTransitionScene("MainMenu")) {
///     // Main menu is now active
/// }
/// 
/// // Update current scene each frame
/// sceneManager.TryUpdateCurrentScene(deltaTime);
/// @endcode
class SceneManager {
public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor.
    SceneManager() = default;

    /// @brief Default destructor.
    /// @details Automatically cleans up all registered scenes and calls OnExit on the current scene if active.
    ~SceneManager() = default;

    // Delete copy operations to ensure single ownership of scenes
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    // Allow move operations
    SceneManager(SceneManager&&) = default;
    SceneManager& operator=(SceneManager&&) = default;

    // Public Methods

    /// @brief Registers a new scene of the specified type with the given name.
    /// @tparam SceneType The type of scene to create. Must inherit from Scene.
    /// @param name Unique name identifier for the scene. Used for name-based lookups.
    /// @return UUID assigned to the registered scene for future reference.
    /// @throws std::runtime_error if a scene with the given name already exists.
    /// @details Creates a new scene instance using SceneType's constructor and stores it
    ///          in the internal registry. The scene name is passed to the constructor along
    ///          with the required ConstructorKey for access control.
    /// @code
    /// auto uuid = sceneManager.RegisterScene<GameplayScene>("Level1");
    /// @endcode
    template<typename SceneType, typename = IsScene<SceneType>>
    Uuid RegisterScene(const std::string& name)
    {
        auto [sceneRef, uuid] = _scenes.EmplaceAs<SceneType>(name, name, Scene::ConstructorKey{});
        return uuid;
    }

    /// @brief Attempts to transition to a scene identified by UUID.
    /// @param uuid UUID of the target scene to transition to.
    /// @return true if the scene was found and transition was successful, false otherwise.
    /// @details If a current scene is active, its OnExit() method is called before transition.
    ///          The target scene's OnEnter() method is called after the transition.
    ///          If the target scene is the same as the current scene, OnExit/OnEnter are still called.
    /// @note This method will not throw exceptions - check return value for success.
    bool TryTransitionScene(const Uuid& uuid);

    /// @brief Attempts to transition to a scene identified by name.
    /// @param name Name of the target scene to transition to.
    /// @return true if the scene was found and transition was successful, false otherwise.
    /// @details If a current scene is active, its OnExit() method is called before transition.
    ///          The target scene's OnEnter() method is called after the transition.
    ///          If the target scene is the same as the current scene, OnExit/OnEnter are still called.
    /// @note This method will not throw exceptions - check return value for success.
    bool TryTransitionScene(const std::string& name);

    /// @brief Attempts to reload the currently active scene.
    /// @return true if there was an active scene to reload, false if no scene is currently active.
    /// @details Effectively calls OnExit() followed by OnEnter() on the current scene.
    ///          This is useful for resetting scene state or reloading after configuration changes.
    ///          If no scene is currently active, this method does nothing and returns false.
    /// @note The scene instance itself is not recreated - only the lifecycle methods are called.
    bool TryReloadCurrentScene();

    /// @brief Attempts to update the currently active scene.
    /// @param deltaTime Time elapsed since the last frame, in seconds.
    /// @return true if there was an active scene to update, false if no scene is currently active.
    /// @details Calls ProcessUpdate(deltaTime) on the current scene if one is active.
    ///          This should typically be called once per frame in the main game loop.
    ///          The ProcessUpdate method handles both scene-specific logic and ECS system updates.
    bool TryUpdateCurrentScene(const float deltaTime);

    /// @brief Attempts to clean up the currently active scene by processing entity destruction queue.
    /// @return true if there was an active scene to clean up, false if no scene is currently active.
    /// @details Processes all entities marked for destruction in the current scene's ECS registry.
    ///          This should typically be called after TryUpdateCurrentScene() each frame to ensure
    ///          proper cleanup of destroyed entities before the next frame begins.
    /// @note This method is essential for proper ECS lifecycle management.
    bool TryCleanupCurrentScene();

    /// @brief Gets the currently active scene.
    /// @return Pointer to the active scene, or nullptr if no scene is currently active.
    /// @details The returned pointer is safe to use as long as the SceneManager exists
    ///          and no scene transitions occur. Do not store this pointer long-term.
    inline Scene* GetCurrentScene() const { return _currentScene; }

    /// @brief Checks if there is currently an active scene.
    /// @return true if a scene is currently active, false otherwise.
    /// @details Equivalent to checking if GetCurrentScene() returns a non-null pointer.
    inline bool HasActiveScene() const { return _currentScene != nullptr; }

    /// @brief Gets the number of registered scenes.
    /// @return Total number of scenes currently registered in the manager.
    inline size_t GetSceneCount() const { return _scenes.Size(); }

    /// @brief Checks if the scene registry is empty.
    /// @return true if no scenes are registered, false otherwise.
    inline bool IsEmpty() const { return _scenes.Empty(); }

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    NameUuidRegistry<Scene> _scenes;
    Scene* _currentScene{nullptr};

    // Private Methods

    bool TryTransitionScene(Scene* const scene);
};

} // namespace velecs::ecs
