#pragma once

#include "velecs/ecs/Object.hpp"
#include "velecs/ecs/TypeConstraints.hpp"

#include <velecs/common/NameUuidRegistry.hpp>
#include <velecs/common/Uuid.hpp>
using velecs::common::NameUuidRegistry;
using velecs::common::Uuid;

#include <algorithm>
#include <memory>
#include <string>

namespace velecs::ecs {

class World;
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
    // Public Fields

    // Constructors and Destructors

    inline SceneManager(World* const world)
        : _world(world) {}

    /// @brief Default constructor.
    SceneManager() = delete;

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

    template<typename SceneT, typename = IsScene<SceneT>>
    SceneT* RegisterScene(const std::string& name, std::optional<size_t> systemCapacity = std::nullopt)
    {
        if (name.empty() || std::all_of(name.begin(), name.end(), [](char c) { return std::isspace(c); }))
        {
            throw std::invalid_argument("Scene name cannot be empty or contain only whitespace");
        }

        // Create concrete type but immediately upcast for storage
        std::unique_ptr<SceneT> concreteScene = systemCapacity.has_value()
            ? std::make_unique<SceneT>(_world, name, systemCapacity.value(), Object::ConstructorKey{})
            : std::make_unique<SceneT>(_world, name, Object::ConstructorKey{});
        
        auto* ptr = concreteScene.get();
        ptr->_uuid = Uuid::GenerateRandom();
        
        // Store as Scene base type
        std::unique_ptr<Scene> sceneBase(concreteScene.release());
        _world->Internal_Register<Scene>(std::move(sceneBase));
        
        return ptr;
    }

    /// @brief Requests a scene transition to the scene identified by UUID.
    /// @param uuid UUID of the target scene to transition to.
    /// @return true if the scene was found and transition was queued, false otherwise.
    /// @details Queues a scene transition request. The actual transition will occur during
    ///          the next call to EngineAccessOnlyTryTransitionIfRequested(). If a transition
    ///          is already pending, this will override the previous request with a warning.
    bool TryRequestSceneTransition(const Uuid& uuid);

    /// @brief Requests a scene transition to the scene identified by name.
    /// @param name Name of the target scene to transition to.
    /// @return true if the scene was found and transition was queued, false otherwise.
    /// @details Queues a scene transition request. The actual transition will occur during
    ///          the next call to EngineAccessOnlyTryTransitionIfRequested(). If a transition
    ///          is already pending, this will override the previous request with a warning.
    bool TryRequestSceneTransition(const std::string& name);

    /// @brief Requests a scene transition to the specified scene.
    /// @param scene Pointer to the target scene to transition to.
    /// @return true if the scene is valid and transition was queued, false otherwise.
    /// @details Queues a scene transition request. The actual transition will occur during
    ///          the next call to EngineAccessOnlyTryTransitionIfRequested(). If a transition
    ///          is already pending, this will override the previous request with a warning.
    bool TryRequestSceneTransition(Scene* const scene);

    /// @brief Attempts to reload the currently active scene.
    /// @return true if there was an active scene to reload, false if no scene is currently active.
    /// @details Queues a reload request for the current scene. Effectively queues OnExit() 
    ///          followed by OnEnter() to be called during the next engine update.
    ///          This is useful for resetting scene state or reloading after configuration changes.
    ///          If no scene is currently active, this method does nothing and returns false.
    bool TryRequestCurrentSceneReload();

    /// @brief Gets the currently active scene.
    /// @return Pointer to the active scene, or nullptr if no scene is currently active.  
    /// @details The returned pointer is safe to use as long as the SceneManager exists
    ///          and no scene transitions occur. Do not store this pointer long-term.
    inline Scene* GetCurrentScene() { return _currentScene; }

    /// @brief Gets the currently active scene (const version).
    /// @return Const pointer to the active scene, or nullptr if no scene is currently active.
    /// @details The returned pointer is safe to use as long as the SceneManager exists
    ///          and no scene transitions occur. Do not store this pointer long-term.
    inline const Scene* GetCurrentScene() const { return _currentScene; }

    /// @brief Checks if there is currently an active scene.
    /// @return true if a scene is currently active, false otherwise.
    /// @details Equivalent to checking if GetCurrentScene() returns a non-null pointer.
    inline bool HasActiveScene() const { return _currentScene != nullptr; }

    /// @brief Gets the number of registered scenes.
    /// @return Total number of scenes currently registered in the world.
    size_t GetSceneCount() const;

    /// @brief Checks if the scene registry is empty.
    /// @return true if no scenes are registered, false otherwise.
    bool IsEmpty() const;
    

    /// #############################################################
    /// ### DO NOT CALL - ENGINE-ONLY FUNCTIONS #####################
    /// #############################################################

    /// @brief Processes any pending scene transition requests. FOR ENGINE USE ONLY.
    /// @param context Execution context data passed to the scene's OnEnter method.
    /// @return true if a transition was processed, false if no transition was pending.
    /// @details This method should ONLY be called by the main engine update loop.
    ///          It handles the actual scene transition by calling OnExit on the current scene
    ///          (if any) and OnEnter on the target scene with the provided context.
    ///          This allows vital data like deltaTime and world references to be passed to scenes
    ///          OnExit and OnEnter implementations.
    /// @warning This method must be called at the beginning of the engine update loop,
    ///          before any scene processing methods.
    bool Internal_TryTransitionIfRequested(void* context);

    /// @brief Processes all enabled systems in the main logic phase for the current scene.
    /// @param context Execution context data passed to each system.
    /// @return true if processing succeeded, false if no active scene.
    bool Internal_TryProcess(void* context);

    /// @brief Processes all enabled systems in the physics phase for the current scene.
    /// @param context Execution context data passed to each system.
    /// @return true if processing succeeded, false if no active scene.
    bool Internal_TryProcessPhysics(void* context);

    /// @brief Processes all enabled systems in the GUI rendering phase for the current scene.
    /// @param context Execution context data passed to each system.
    /// @return true if processing succeeded, false if no active scene.
    bool Internal_TryProcessGUI(void* context);

    /// @brief Processes cleanup and entity destruction for the current scene.
    /// @return true if processing succeeded, false if no active scene.
    bool Internal_TryProcessEntityCleanup();

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    World* _world;
    Scene* _currentScene{nullptr};
    Scene* _targetScene{nullptr};

    // Private Methods
};

} // namespace velecs::ecs
