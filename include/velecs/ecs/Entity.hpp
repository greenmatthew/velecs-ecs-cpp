/// @file    Entity.hpp
/// @author  Matthew Green
/// @date    2025-05-12 16:24:57
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include "velecs/ecs/TypeConstraints.hpp"
#include "velecs/ecs/Scene.hpp"
#include "velecs/ecs/Component.hpp"

#include <iostream>
#include <functional>

#include <entt/entt.hpp>

namespace velecs::ecs {

class Scene;
class EntityBuilder;

class Name;
class Transform;

/// @class Entity
/// @brief Brief description.
///
/// Rest of description.
class Entity {
    friend class Scene;
    friend class EntityBuilder;
    friend class Component;

public:
    // Enums

    // Public Fields

    static const Entity INVALID;

    // Constructors and Destructors

    /// @brief Constructor with handle.
    /// @param scene The scene that the entity will be apart of.
    /// @param handle The entity handle to use.
    explicit Entity(Scene* const scene, const entt::entity handle);

    /// @brief Default deconstructor.
    ~Entity() = default;

    /// @brief Copy constructor
    /// @param other The entity to copy from
    inline Entity(const Entity& other)
        : _scene(other._scene), _handle(other._handle) {}

    /// @brief Creates a new entity in the registry.
    /// @return A newly created entity with a valid handle.
    static EntityBuilder Create(Scene* const scene);

    // Public Methods

    /// @brief Copy assignment operator.
    /// @param other The entity to copy from.
    /// @return A reference to this entity after assignment.
    inline Entity& operator=(const Entity& other) noexcept
    {
        if (this != &other) // Check for self-assignment
        {
            // Since handle is const to prevent accidental modification,
            // we need to use const_cast here to enable the desired
            // reassignment behavior
            const_cast<entt::entity&>(_handle) = other._handle; // Copy handle
        }
        return *this;
    }

    Entity& operator=(const EntityBuilder& other);

    /// @brief Equality comparison operator.
    /// @param other The entity to compare with.
    /// @return True if the entities have the same handle, false otherwise.
    inline bool operator==(const Entity& other) const { return _handle == other._handle; }

    /// @brief Inequality comparison operator.
    /// @param other The entity to compare with.
    /// @return True if the entities have different handles, false otherwise.
    inline bool operator!=(const Entity& other) const { return _handle != other._handle; }

    /// @brief Boolean conversion operator.
    /// @details Allows the entity to be used in conditional expressions.
    ///          An entity evaluates to true if it is valid, false otherwise.
    /// @return True if the entity is valid, false otherwise.
    inline explicit operator bool() const { return IsValid(); }

    /// @brief Checks if the entity is valid.
    /// @return True if the entity's scene pointer is not null and it's handle is valid in the registry, false otherwise.
    bool IsValid() const;

    /// @brief Gets the name of this entity.
    /// @details Retrieves the name stored in the Name component of this entity.
    /// @return A string containing the entity's name.
    const std::string& GetName() const;

    /// @brief Sets the name of this entity.
    /// @details Updates the name stored in the Name component of this entity.
    /// @param newName The new name to assign to this entity.
    void SetName(const std::string& newName);

    /// @brief Gets the Transform component of this entity.
    /// @details This is a convenience method that assumes the entity has a Transform component.
    ///          This method should only be called when you know the entity has a Transform.
    /// @return A reference to the entity's Transform component.
    Transform& GetTransform() const;

    /// @brief 
    /// @tparam ComponentType
    /// @tparam  
    template<typename TagType, typename = IsTag<TagType>>
    void AddTag()
    {
        _scene->AddTag<TagType>(*this);
    }

    /// @brief Adds a component of type ComponentType to the entity.
    /// @tparam ComponentType The type of component to add.
    /// @return A reference to the newly added component.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    ComponentType& AddComponent()
    {
        return _scene->AddComponent<ComponentType>(*this);
    }

    /// @brief Adds a component of type ComponentType to the entity with constructor arguments.
    /// @tparam ComponentType The type of component to add.
    /// @tparam Args The types of the constructor arguments.
    /// @param args The constructor arguments.
    /// @return A reference to the newly added component.
    template<typename ComponentType, typename = IsComponent<ComponentType>, typename... Args>
    ComponentType& AddComponent(Args &&...args)
    {
        return _scene->AddComponent<ComponentType>(*this, std::forward<Args>(args)...);
    }

    /// @brief Removes a component of type T from the entity.
    /// @tparam ComponentType The type of component to remove.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    void RemoveComponent()
    {
        return _scene->RemoveComponent(*this);
    }

    /// @brief Tries to get a component of type T from the entity.
    /// @tparam ComponentType The type of component to get.
    /// @param outComponent A pointer that will be set to the component if found.
    /// @return True if the component was found, false otherwise.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool TryGetComponent(ComponentType*& outComponent)
    {
        return _scene->TryGetComponent(*this, outComponent);
    }

    /// @brief Tries to get a const component of type T from the entity.
    /// @tparam ComponentType The type of component to get.
    /// @param outComponent A const pointer that will be set to the component if found.
    /// @return True if the component was found, false otherwise.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool TryGetComponent(const ComponentType*& outComponent) const
    {
        return _scene->TryGetComponent(*this, outComponent);
    }

    inline size_t GetHashCode() const { return std::hash<entt::entity>{}(_handle); }

    /// @brief Requests an entity to be destroyed.
    /// @details This adds the entity to a queue for destruction rather than destroying it immediately.
    /// @param entity The entity to be queued for destruction.
    static void RequestDestroy(Entity entity);

    /// @brief Processes all pending entity destruction requests.
    /// @details Iterates through the destruction queue and destroys each entity.
    static void ProcessDestructionQueue();

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    Scene* const _scene;
    const entt::entity _handle;

    // Private Methods

    /// @brief Default constructor.
    /// @details Creates an entity with an invalid handle.
    inline explicit Entity()
        : _scene(nullptr), _handle(entt::null) {}
};

} // namespace velecs::ecs


namespace std {

template <>
struct hash<velecs::ecs::Entity> {
    size_t operator()(const velecs::ecs::Entity& entity) const
    {
        return entity.GetHashCode();
    }
};

} // namespace std