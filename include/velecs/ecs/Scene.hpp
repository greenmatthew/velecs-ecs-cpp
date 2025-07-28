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

class Entity;
class EntityBuilder;

/// @class Scene
/// @brief Brief description.
///
/// Rest of description.
class Scene {
public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    struct ConstructorKey {
        // friend class SceneManager;
        ConstructorKey() = default;
    };

    inline Scene(const std::string& name)
        : _name(name)
    {
        std::cerr << "Remove this once SceneManager is working." << std::endl;
    }

    Scene(const std::string& name, ConstructorKey);

    /// @brief Default constructor.
    Scene() = delete;

    /// @brief Default deconstructor.
    virtual ~Scene() = 0;

    // Public Methods

    virtual void OnEnter() {}
    virtual void OnExit() {}

    bool IsEntityValid(const Entity entity);
    EntityBuilder CreateEntity();

    /// @brief 
    /// @tparam TagType 
    /// @tparam  
    /// @param entity 
    template<typename TagType, typename = IsTag<TagType>>
    void AddTag(const Entity entity)
    {
        _registry.emplace<TagType>(entity._handle);
    }

    /// @brief Adds a component of type T to the entity.
    /// @tparam ComponentType The type of component to add.
    /// @return A reference to the newly added component.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    ComponentType& AddComponent(const Entity entity)
    {
        ComponentType& comp = _registry.emplace<ComponentType>(entity._handle);
        comp._scene = this;
        comp._handle = entity._handle;
        return comp;
    }

    /// @brief Adds a component of type T to the entity with constructor arguments.
    /// @tparam ComponentType The type of component to add.
    /// @tparam Args The types of the constructor arguments.
    /// @param args The constructor arguments.
    /// @return A reference to the newly added component.
    template<typename ComponentType, typename = IsComponent<ComponentType>, typename... Args>
    ComponentType& AddComponent(const Entity entity, Args &&...args)
    {
        ComponentType& comp = _registry.emplace<ComponentType>(entity._handle, std::forward<Args>(args)...);
        comp._scene = this;
        comp._handle = entity._handle;
        return comp;
    }

    /// @brief Removes a component of type T from the entity.
    /// @tparam ComponentType The type of component to remove.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    void RemoveComponent(const Entity entity)
    {
        _registry.remove<ComponentType>(entity._handle);
    }

    /// @brief Tries to get a mutable component from the entity (non-const Scene method).
    /// @tparam ComponentType The type of component to get.
    /// @param entity The entity to get the component from.
    /// @param outComponent A pointer that will be set to the component if found.
    /// @return True if the component was found, false otherwise.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool TryGetComponent(const Entity entity, ComponentType*& outComponent)
    {
        outComponent = _registry.try_get<ComponentType>(entity._handle);
        return (outComponent != nullptr);
    }

    /// @brief Tries to get a const component from the entity (const Scene method).
    /// @tparam ComponentType The type of component to get.
    /// @param entity The entity to get the component from.
    /// @param outComponent A const pointer that will be set to the component if found.
    /// @return True if the component was found, false otherwise.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool TryGetComponent(const Entity entity, const ComponentType*& outComponent) const
    {
        outComponent = _registry.try_get<ComponentType>(entity._handle);
        return (outComponent != nullptr);
    }

    void RegisterSystem();

    void ProcessInit();
    void ProcessUpdate(float deltaTime);
    void ProcessCleanup();

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields
    using MarkedView = entt::basic_view<entt::get_t<entt::constness_as_t<entt::storage_type_t<velecs::ecs::DestroyTag, entt::entity, std::allocator<velecs::ecs::DestroyTag>>, velecs::ecs::DestroyTag>>, entt::exclude_t<>, void>;

    std::string _name;
    entt::registry _registry;

    MarkedView* _markedView;

    // Private Methods
};

} // namespace velecs::ecs
