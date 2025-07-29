/// @file    Scene.hpp
/// @author  Matthew Green
/// @date    2025-07-27 19:03:33
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include "velecs/ecs/Tags/DestroyTag.hpp"

#include <entt/entt.hpp>

#include <string>
#include <deque>
#include <iostream>

namespace velecs::ecs {

class SceneManager;

class Entity;
class EntityBuilder;

/// @class Scene
/// @brief Represents a self-contained game scene with its own entity registry and lifecycle management.
///
/// A Scene manages a collection of entities and their components within a specific context
/// (e.g., main menu, gameplay level, pause screen). Each scene maintains its own EnTT registry,
/// allowing for complete isolation between different game states. Scenes provide lifecycle
/// hooks (OnEnter/OnExit) for setup and cleanup, and handle entity creation, component
/// management, and system processing. This design enables easy scene transitions and
/// modular game architecture.
class Scene {
    friend class SceneManager;
public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Constructor access key to enforce controlled scene creation.
    /// @details This prevents direct instantiation and ensures scenes are created
    ///          through proper management systems (like SceneManager).
    class ConstructorKey {
        friend class SceneManager;
        ConstructorKey() = default;
    public:
        virtual ~ConstructorKey() = default;
    };

    /// @brief Primary constructor for scene creation.
    /// @param name The name identifier for this scene.
    /// @param key Constructor access key to control instantiation.
    Scene(const std::string& name, ConstructorKey key);

    /// @brief Deleted default constructor.
    /// @details Scenes must always have a name identifier.
    Scene() = delete;

    /// @brief Pure virtual destructor to make Scene abstract.
    /// @details Ensures proper cleanup of derived scene types and makes
    ///          this class abstract so it cannot be instantiated directly.
    virtual ~Scene() = 0;

    // Public Methods

    /// @brief Called when the scene becomes active.
    /// @details Override this method in derived classes to perform scene-specific
    ///          initialization such as loading assets, setting up UI, or spawning
    ///          initial entities. Default implementation does nothing.
    virtual void OnEnter() {}

    /// @brief Called when the scene becomes inactive.
    /// @details Override this method in derived classes to perform scene-specific
    ///          cleanup such as saving state, unloading assets, or clearing temporary
    ///          data. Default implementation does nothing.
    virtual void OnExit() {}

    /// @brief Checks if an entity's handle is valid within this scene's registry.
    /// @param entity The entity to validate.
    /// @return True if the entity's handle is not null and exists in this scene's registry, false otherwise.
    bool IsEntityHandleValid(const Entity entity);

    /// @brief Creates a new entity within this scene.
    /// @return An EntityBuilder for configuring the newly created entity.
    /// @details The returned EntityBuilder provides a fluent interface for adding
    ///          components and configuring the entity before use.
    EntityBuilder CreateEntity();

    /// @brief Adds a tag of the specified type to an entity.
    /// @tparam TagType The type of tag to add. Must inherit from Tag.
    /// @param entity The entity to add the tag to.
    /// @details Tags are empty components used for categorization and filtering.
    ///          They provide no data but can be used in queries and systems.
    template<typename TagType, typename = IsTag<TagType>>
    void AddTag(const Entity entity)
    {
        _registry.emplace<TagType>(entity._handle);
    }

    /// @brief Adds a component of the specified type to an entity.
    /// @tparam ComponentType The type of component to add. Must inherit from Component.
    /// @param entity The entity to add the component to.
    /// @return A reference to the newly added component.
    /// @details The component is default-constructed and its scene/handle are automatically set.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    ComponentType& AddComponent(const Entity entity)
    {
        assert(this != nullptr && "Scene must be valid");
        assert(entity._handle != entt::null || _registry.valid(entity._handle) && "Scene must be valid");
        ComponentType& comp = _registry.emplace<ComponentType>(entity._handle);
        comp._scene = this;
        comp._handle = entity._handle;
        return comp;
    }

    /// @brief Adds a component of the specified type to an entity with constructor arguments.
    /// @tparam ComponentType The type of component to add. Must inherit from Component.
    /// @tparam Args The types of the constructor arguments.
    /// @param entity The entity to add the component to.
    /// @param args The constructor arguments to forward to the component constructor.
    /// @return A reference to the newly added component.
    /// @details The component is constructed with the provided arguments and its scene/handle are automatically set.
    template<typename ComponentType, typename = IsComponent<ComponentType>, typename... Args>
    ComponentType& AddComponent(const Entity entity, Args &&...args)
    {
        assert(this != nullptr && "Scene must be valid");
        assert(entity._handle != entt::null || _registry.valid(entity._handle) && "Scene must be valid");
        ComponentType& comp = _registry.emplace<ComponentType>(entity._handle, std::forward<Args>(args)...);
        comp._scene = this;
        comp._handle = entity._handle;
        return comp;
    }

    /// @brief Removes a component of the specified type from an entity.
    /// @tparam ComponentType The type of component to remove. Must inherit from Component.
    /// @param entity The entity to remove the component from.
    /// @details Does nothing if the entity doesn't have the specified component.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    void RemoveComponent(const Entity entity)
    {
        _registry.remove<ComponentType>(entity._handle);
    }

    /// @brief Tries to get a mutable component from an entity.
    /// @tparam ComponentType The type of component to get. Must inherit from Component.
    /// @param entity The entity to get the component from.
    /// @param outComponent A pointer that will be set to the component if found, or nullptr if not found.
    /// @return True if the component was found, false otherwise.
    /// @details This non-const overload allows modification of the retrieved component.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool TryGetComponent(const Entity entity, ComponentType*& outComponent)
    {
        outComponent = _registry.try_get<ComponentType>(entity._handle);
        return (outComponent != nullptr);
    }

    /// @brief Tries to get a const component from an entity.
    /// @tparam ComponentType The type of component to get. Must inherit from Component.
    /// @param entity The entity to get the component from.
    /// @param outComponent A const pointer that will be set to the component if found, or nullptr if not found.
    /// @return True if the component was found, false otherwise.
    /// @details This const overload provides read-only access to the retrieved component.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool TryGetComponent(const Entity entity, const ComponentType*& outComponent) const
    {
        outComponent = _registry.try_get<ComponentType>(entity._handle);
        return (outComponent != nullptr);
    }

    /// @brief Registers a system with this scene.
    /// @details Implementation pending. Will handle system registration and lifecycle management.
    /// @todo Implement system registration functionality.
    void RegisterSystem();

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields
    
    std::string _name;              ///< @brief The name identifier for this scene.
    entt::registry _registry;       ///< @brief The EnTT registry managing entities and components for this scene.

    // Private Methods

    void DestroyEntity(Entity entity);

    /// @brief Initializes all systems in this scene.
    /// @details Called once when the scene becomes active to set up system state.
    /// @todo Implement system initialization processing.
    void ProcessInit();

    /// @brief Updates all systems in this scene.
    /// @param deltaTime Time elapsed since the last frame in seconds.
    /// @details Called every frame to update system logic and process entities.
    /// @todo Implement system update processing.
    void ProcessUpdate(float deltaTime);

    /// @brief Processes cleanup and entity destruction for this scene.
    /// @details Handles deferred entity destruction and system cleanup.
    ///          Should be called at the end of each frame.
    /// @todo Complete entity destruction queue implementation.
    void ProcessCleanup();
};

} // namespace velecs::ecs
