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
#include "velecs/ecs/TypeConstraints.hpp"

#include <velecs/common/Context.hpp>
using velecs::common::Context;

#include <entt/entt.hpp>

#include <string>
#include <deque>
#include <iostream>
#include <optional>
#include <typeindex>

namespace velecs::ecs {

class SceneManager;

class Entity;
class EntityBuilder;
class Component;
class System;

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

private:
    /// @brief ID for a System
    using SystemId = std::type_index;
    /// @brief Storage for a System
    using SystemStorage = std::unique_ptr<System>;

public:
    // Enums

    // Public Fields

    /// @brief Default capacity for system storage allocation.
    /// @details Scenes reserve this many system slots by default to avoid reallocations
    ///          during typical system registration.
    static const size_t DEFAULT_SYSTEM_CAPACITY = 128;

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

    /// @brief Constructor for scene creation with custom system capacity.
    /// @param name The name identifier for this scene.
    /// @param systemCapacity Number of systems to reserve space for during initialization.
    /// @param key Constructor access key to control instantiation.
    /// @details Creates a scene with pre-allocated system storage to avoid reallocations
    ///          during system registration. Use this constructor for scenes that will
    ///          have a known large number of systems for optimal performance.
    Scene(const std::string& name, size_t systemCapacity, ConstructorKey key);

    /// @brief Primary constructor for scene creation with default system capacity.
    /// @param name The name identifier for this scene.
    /// @param key Constructor access key to control instantiation.
    /// @details Creates a scene with default system storage capacity (DEFAULT_SYSTEM_CAPACITY).
    ///          This constructor delegates to the capacity-based constructor and is suitable
    ///          for most scenes. For scenes with many systems, consider using the capacity
    ///          constructor for better performance.
    Scene(const std::string& name, ConstructorKey key);

    /// @brief Deleted default constructor.
    /// @details Scenes must always have a name identifier.
    Scene() = delete;

    /// @brief Pure virtual destructor to make Scene abstract.
    /// @details Ensures proper cleanup of derived scene types and makes
    ///          this class abstract so it cannot be instantiated directly.
    virtual ~Scene() = 0;

    // Public Methods

    inline std::string GetName() const { return _name; }

    /// @brief Called when the scene becomes active.
    /// @details Override this method in derived classes to perform scene-specific
    ///          initialization such as loading assets, setting up UI, or spawning
    ///          initial entities. Default implementation does nothing.
    virtual void OnEnter(void* context) {}

    /// @brief Called when the scene becomes inactive.
    /// @details Override this method in derived classes to perform scene-specific
    ///          cleanup such as saving state, unloading assets, or clearing temporary
    ///          data. Default implementation does nothing.
    virtual void OnExit(void* context) {}



    // ========== Entity Management ==========



    /// @brief Checks if an entity's handle is valid within this scene's registry.
    /// @param entity The entity to validate.
    /// @return True if the entity's handle is not null and exists in this scene's registry, false otherwise.
    bool IsEntityHandleValid(const Entity entity);

    /// @brief Creates a new entity within this scene.
    /// @return An EntityBuilder for configuring the newly created entity.
    /// @details The returned EntityBuilder provides a fluent interface for adding
    ///          components and configuring the entity before use.
    EntityBuilder CreateEntity();



    // ========== Tag Management ==========



    /// @brief Checks if an entity has a tag of the specified type.
    /// @tparam TagType The type of tag to check for. Must inherit from Tag.
    /// @param entity The entity to check.
    /// @return True if the entity has the specified tag, false otherwise.
    template<typename TagType, typename = IsTag<TagType>>
    bool HasTag(const Entity entity) const
    {
        return GetRegistry().all_of<TagType>(entity._handle);
    }

    /// @brief Attempts to add a tag of the specified type to an entity.
    /// @tparam TagType The type of tag to add. Must inherit from Tag.
    /// @param entity The entity to add the tag to.
    /// @return True if the tag was successfully added, false if the entity already had the tag.
    /// @details Safe to call even if the entity already has the specified tag.
    template<typename TagType, typename = IsTag<TagType>>
    bool TryAddTag(const Entity entity)
    {
        if (HasTag<TagType>(entity)) return false;
        GetRegistry().emplace<TagType>(entity._handle);
        return true;
    }

    /// @brief Attempts to remove a tag of the specified type from an entity.
    /// @tparam TagType The type of tag to remove. Must inherit from Tag.
    /// @param entity The entity to remove the tag from.
    /// @return True if the tag was successfully removed, false if the entity didn't have the tag.
    /// @details Safe to call even if the entity doesn't have the specified tag.
    template<typename TagType, typename = IsTag<TagType>>
    bool TryRemoveTag(const Entity entity)
    {
        return GetRegistry().remove<TagType>(entity._handle) > 0;
    }



    // ========== Component Management ==========



    /// @brief Checks if an entity has a component of the specified type.
    /// @tparam ComponentType The type of component to check for. Must inherit from Component.
    /// @param entity The entity to check.
    /// @return True if the entity has the specified component, false otherwise.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool HasComponent(const Entity entity) const
    {
        return GetRegistry().all_of<ComponentType>(entity._handle);
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
        assert(entity.IsValid() && "Entity must be valid");
        outComponent = GetRegistry().try_get<ComponentType>(entity._handle);
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
        assert(entity.IsValid() && "Entity must be valid");
        outComponent = GetRegistry().try_get<ComponentType>(entity._handle);
        return (outComponent != nullptr);
    }

    /// @brief Attempts to add a component of the specified type to an entity.
    /// @tparam ComponentType The type of component to add. Must inherit from Component.
    /// @param entity The entity to add the component to.
    /// @param outComponent A pointer that will be set to the component if successfully added, or nullptr if the entity already has the component.
    /// @return True if the component was successfully added, false if the entity already has the component.
    /// @details The component is default-constructed and its scene/handle are automatically set.
    ///          Safe to call even if the entity already has the specified component type.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool TryAddComponent(const Entity entity, ComponentType*& outComponent)
    {
        assert(entity.IsValid() && "Entity must be valid");
        if (HasComponent<ComponentType>(entity))
        {
            outComponent = nullptr;
            return false;
        }

        ComponentType& comp = GetRegistry().emplace<ComponentType>(entity._handle);
        comp._scene = this;
        comp._handle = entity._handle;
        outComponent = &comp;
        return true;
    }

    /// @brief Attempts to add a const component of the specified type to an entity.
    /// @tparam ComponentType The type of component to add. Must inherit from Component.
    /// @param entity The entity to add the component to.
    /// @param outComponent A const pointer that will be set to the component if successfully added, or nullptr if the entity already has the component.
    /// @return True if the component was successfully added, false if the entity already has the component.
    /// @details The component is default-constructed and its scene/handle are automatically set.
    ///          This const overload provides read-only access to the newly added component.
    ///          Safe to call even if the entity already has the specified component type.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool TryAddComponent(const Entity entity, const ComponentType*& outComponent)
    {
        assert(entity.IsValid() && "Entity must be valid");
        if (HasComponent<ComponentType>(entity))
        {
            outComponent = nullptr;
            return false;
        }

        ComponentType& comp = GetRegistry().emplace<ComponentType>(entity._handle);
        comp._scene = this;
        comp._handle = entity._handle;
        outComponent = &comp;
        return true;
    }

    /// @brief Attempts to add a component of the specified type to an entity with constructor arguments.
    /// @tparam ComponentType The type of component to add. Must inherit from Component.
    /// @tparam Args The types of the constructor arguments.
    /// @param entity The entity to add the component to.
    /// @param outComponent A pointer that will be set to the component if successfully added, or nullptr if the entity already has the component.
    /// @param args The constructor arguments to forward to the component constructor.
    /// @return True if the component was successfully added, false if the entity already has the component.
    /// @details The component is constructed with the provided arguments and its scene/handle are automatically set.
    ///          Safe to call even if the entity already has the specified component type.
    template<typename ComponentType, typename = IsComponent<ComponentType>, typename... Args>
    bool TryAddComponent(const Entity entity, ComponentType*& outComponent, Args &&...args)
    {
        assert(entity.IsValid() && "Entity must be valid");
        if (HasComponent<ComponentType>(entity))
        {
            outComponent = nullptr;
            return false;
        }

        ComponentType& comp = GetRegistry().emplace<ComponentType>(entity._handle, std::forward<Args>(args)...);
        comp._scene = this;
        comp._handle = entity._handle;
        outComponent = &comp;
        return true;
    }

    /// @brief Attempts to add a const component of the specified type to an entity with constructor arguments.
    /// @tparam ComponentType The type of component to add. Must inherit from Component.
    /// @tparam Args The types of the constructor arguments.
    /// @param entity The entity to add the component to.
    /// @param outComponent A const pointer that will be set to the component if successfully added, or nullptr if the entity already has the component.
    /// @param args The constructor arguments to forward to the component constructor.
    /// @return True if the component was successfully added, false if the entity already has the component.
    /// @details The component is constructed with the provided arguments and its scene/handle are automatically set.
    ///          This const overload provides read-only access to the newly added component.
    ///          Safe to call even if the entity already has the specified component type.
    template<typename ComponentType, typename = IsComponent<ComponentType>, typename... Args>
    bool TryAddComponent(const Entity entity, const ComponentType*& outComponent, Args &&...args)
    {
        assert(entity.IsValid() && "Entity must be valid");
        if (HasComponent<ComponentType>(entity))
        {
            outComponent = nullptr;
            return false;
        }

        ComponentType& comp = GetRegistry().emplace<ComponentType>(entity._handle, std::forward<Args>(args)...);
        comp._scene = this;
        comp._handle = entity._handle;
        outComponent = &comp;
        return true;
    }

    /// @brief Attempts to remove a component of the specified type from an entity.
    /// @tparam ComponentType The type of component to remove. Must inherit from Component.
    /// @param entity The entity to remove the component from.
    /// @return True if the component was successfully removed, false if the entity didn't have the component.
    /// @details Safe to call even if the entity doesn't have the specified component type.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool TryRemoveComponent(const Entity entity)
    {
        if (!HasComponent<ComponentType>(entity)) return false;
        GetRegistry().remove<ComponentType>(entity._handle);
        return true;
    }
    


    // ========== System Management ==========



    /// @brief Checks if the scene has a system of the specified type.
    /// @tparam SystemType The type of system to check for. Must inherit from System.
    /// @return True if the scene has the specified system, false otherwise.
    /// @details Performs consistency validation between internal system storage collections
    ///          to ensure data structure integrity.
    template<typename SystemType, typename = IsSystem<SystemType>>
    bool HasSystem()
    {
        SystemId id = GetSystemId<SystemType>();
        
        bool inMap = _systems.find(id) != _systems.end();
        auto iteratorIt = std::find(_systemsIterator.begin(), _systemsIterator.end(), id);
        bool inIterator = (iteratorIt != _systemsIterator.end());
        
        // Ensure both collections are synchronized
        assert(inMap == inIterator && 
            "System storage collections out of sync - system found in one collection but not the other");
        
        return inMap;
    }

    /// @brief Attempts to register a system of the specified type with default construction.
    /// @tparam SystemType The type of system to add. Must inherit from System.
    /// @return true if the system was successfully added, false if a system of this type already exists.
    /// @details Creates a new system instance using default constructor, calls Init() on it,
    ///          and stores it in the scene's system registry in execution order. Only one system 
    ///          of each type is allowed per scene. Systems are automatically sorted by their
    ///          GetExecutionOrder() value during insertion.
    template<typename SystemType, typename = IsSystem<SystemType>>
    bool TryAddSystem()
    {
        return TryAddSystem(std::make_unique<SystemType>());
    }

    /// @brief Attempts to register a system of the specified type with constructor arguments.
    /// @tparam SystemType The type of system to add. Must inherit from System.
    /// @tparam Args The types of the constructor arguments.
    /// @param args The constructor arguments to forward to the system constructor.
    /// @return true if the system was successfully added, false if a system of this type already exists.
    /// @details Creates a new system instance with the provided constructor arguments,
    ///          calls Init() on it, and stores it in the scene's system registry in execution order.
    ///          Only one system of each type is allowed per scene. Systems are automatically
    ///          sorted by their GetExecutionOrder() value during insertion.
    template<typename SystemType, typename = IsSystem<SystemType>, typename... Args>
    bool TryAddSystem(Args&&... args)
    {
        return TryAddSystem(std::make_unique<SystemType>(std::forward<Args>(args)...));
    }

    /// @brief Attempts to remove a system of the specified type from the scene.
    /// @tparam SystemType The type of system to remove. Must inherit from System.
    /// @return true if the system was successfully removed, false if no system of this type exists.
    /// @details Calls Cleanup() on the system before removing it from both the system registry
    ///          and the execution order list. This ensures proper resource cleanup and maintains
    ///          data structure consistency.
    template<typename SystemType, typename = IsSystem<SystemType>>
    bool TryRemoveSystem()
    {
        SystemId id = GetSystemId<SystemType>();
    
        auto it = _systems.find(id);
        if (it == _systems.end()) return false;
        
        // Call cleanup before removal
        it->second->Cleanup();

        // Remove from map
        _systems.erase(it);
        
        // Find and remove from iterator vector (simple linear search)
        auto iteratorIt = std::find(_systemsIterator.begin(), _systemsIterator.end(), id);
        assert(iteratorIt != _systemsIterator.end() && *iteratorIt == id && 
               "System found in _systems map but not in _systemsIterator - data structure consistency violated");
        
        if (iteratorIt != _systemsIterator.end()) _systemsIterator.erase(iteratorIt);
        
        return true;
    }

    /// @brief Gets the system ID for a given system type.
    /// @tparam SystemType The type of system to get the ID for. Must inherit from System.
    /// @return The type_index used as the system identifier.
    /// @details This provides a consistent way to identify system types across the ECS.
    ///          The ID is used internally for system storage and lookup operations.
    template<typename SystemType, typename = IsSystem<SystemType>>
    SystemId GetSystemId()
    {
        return typeid(SystemType);
    }

    // Single component with perfect forwarding
    template<typename TagOrComponent, typename Func>
    void Query(Func&& callback)
    {
        auto& registry = GetRegistry();
        registry.view<Component>().each([this, &callback](entt::entity entity, TagOrComponent& tagOrComp) {
            Entity wrappedEntity{this, entity};
            callback(wrappedEntity, tagOrComp);
        });
    }

    // Multi-component with perfect forwarding  
    template<typename... TagsOrComponents, typename Func>
    void Query(Func&& callback)
    {
        static_assert(sizeof...(TagsOrComponents) > 1, "Use single component Query overload for single component queries");
        
        GetRegistry().view<TagsOrComponents...>().each([this, callback = std::forward<Func>(callback)](entt::entity e, TagsOrComponents&... tagsOrComps) {
            Entity entity{this, e};
            callback(entity, tagsOrComps...);
        });
    }

    template<typename TagOrComponent>
    void Query(std::function<void(Entity, TagOrComponent&)> callback)
    {
        auto& registry = GetRegistry();
        auto view = registry.view<TagOrComponent>();
        
        for (auto entity : view) {
            Entity wrappedEntity{this, entity};
            TagOrComponent& tagOrComp = registry.get<TagOrComponent>(entity);
            callback(wrappedEntity, tagOrComp);
        }
    }

    template<typename... TagsOrComponents>
    void Query(std::function<void(Entity, TagsOrComponents&...)> callback)
    {
        static_assert(sizeof...(TagsOrComponents) > 1, "Use single component Query overload for single component queries");
        
        GetRegistry().view<TagsOrComponents...>().each([this, &callback](entt::entity e, TagsOrComponents&... tagsOrComps) {
            Entity entity{this, e};
            callback(entity, tagsOrComps...);
        });
    }

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields
    
    std::string _name;                                    ///< @brief The name identifier for this scene.
    std::optional<entt::registry> _registry;              ///< @brief The EnTT registry managing entities and components for this scene.

    /// @brief Ordered list of system type indices for deterministic iteration.
    std::vector<SystemId> _systemsIterator;
    /// @brief Map of system type indices to system instances for fast lookup and storage.
    std::unordered_map<SystemId, SystemStorage> _systems;

    // Private Methods

    /// @brief Gets a reference to the scene's entity registry.
    /// @return Reference to the active EnTT registry.
    /// @throws std::runtime_error if the registry has not been initialized.
    /// @details Provides access to the scene's entity registry for internal operations.
    ///          The registry is only valid between Init() and Cleanup() calls.
    entt::registry& GetRegistry();

    /// @brief Gets a const reference to the scene's entity registry.
    /// @return Const reference to the active EnTT registry.
    /// @throws std::runtime_error if the registry has not been initialized.
    /// @details Provides read-only access to the scene's entity registry.
    const entt::registry& GetRegistry() const;

    /// @brief Initializes the scene's entity registry and calls OnEnter().
    /// @details Creates the EnTT registry for this scene and triggers the OnEnter()
    ///          lifecycle method. Called automatically by SceneManager during scene transitions.
    void Init(void* context);

    /// @brief Cleans up the scene's entity registry and calls OnExit().
    /// @details Triggers the OnExit() lifecycle method, clears all entities from the registry,
    ///          and resets the registry. Called automatically by SceneManager during scene transitions.
    void Cleanup(void* context);
    
    /// @brief Attempts to register a pre-constructed system instance with the scene.
    /// @tparam SystemType The type of system to add. Must inherit from System.
    /// @param system A unique_ptr to the system instance to register.
    /// @return True if the system was successfully added, false if a system of this type already exists.
    /// @details This is the core system registration method used by other TryAddSystem overloads.
    ///          Takes ownership of the provided system instance, calls Init() on it, and stores it
    ///          in the scene's system registry. Systems are automatically sorted by their
    ///          GetExecutionOrder() value during insertion to maintain deterministic execution order.
    ///          Only one system of each type is allowed per scene.
    template<typename SystemType, typename = IsSystem<SystemType>>
    inline bool TryAddSystem(std::unique_ptr<SystemType> system)
    {
        SystemId id = GetSystemId<SystemType>();
        
        if (_systems.find(id) != _systems.end()) return false;
        
        auto [it, inserted] = _systems.emplace(id, std::move(system));
        assert(inserted && "System emplace failed after find() check");

        // Sorted insertion using execution order
        auto insertPos = std::lower_bound(_systemsIterator.begin(), _systemsIterator.end(), id,
            [this](const SystemId& a, const SystemId& b) {
                return _systems.at(a)->GetExecutionOrder() < _systems.at(b)->GetExecutionOrder();
            });
        _systemsIterator.insert(insertPos, id);

        it->second->Init();
        return inserted;
    }

    /// @brief Destroys a specific entity from this scene's registry.
    /// @param entity The entity to destroy.
    /// @details Immediately removes the entity and all its components from the registry.
    ///          This is used internally by the cleanup system for deferred entity destruction.
    void DestroyEntity(Entity entity);

    /// @brief Processes all enabled systems in the main logic phase.
    /// @param context Execution context data passed to each system (currently void* placeholder).
    /// @details Iterates through all registered systems in execution order and calls their
    ///          Process() method if they are enabled. This is the primary system update phase
    ///          where most game logic occurs. Disabled systems are skipped automatically.
    void Process(void* context);

    /// @brief Processes all enabled systems in the physics simulation phase.
    /// @param context Execution context data passed to each system (currently void* placeholder).
    /// @details Iterates through all registered systems in execution order and calls their
    ///          ProcessPhysics() method if they are enabled. This phase typically runs after
    ///          the main logic phase to handle physics-related updates and calculations.
    void ProcessPhysics(void* context);

    /// @brief Processes all enabled systems in the GUI rendering phase.
    /// @param context Execution context data passed to each system (currently void* placeholder).
    /// @details Iterates through all registered systems in execution order and calls their
    ///          ProcessGUI() method if they are enabled. This phase typically runs last to
    ///          ensure UI reflects the current game state after all logic and physics updates.
    void ProcessGUI(void* context);

    /// @brief Processes cleanup and entity destruction for this scene.
    /// @details Handles deferred entity destruction and system cleanup.
    ///          Should be called at the end of each frame.
    void ProcessEntityCleanup();
};

} // namespace velecs::ecs
