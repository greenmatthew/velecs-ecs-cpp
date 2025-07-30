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

#include <entt/entt.hpp>

#include <iostream>
#include <functional>

namespace velecs::ecs {

class Scene;
class EntityBuilder;

class Name;
class Transform;

/// @class Entity
/// @brief Represents an entity in the Entity-Component-System (ECS) architecture.
///
/// An Entity is a unique identifier that can have components attached to it.
/// Entities themselves contain no data or behavior - they serve as containers
/// for components which contain data. This class provides
/// a convenient interface for managing entities within a scene, including
/// adding/removing components, accessing common components like Name and Transform,
/// and managing entity lifecycle.
class Entity {
    friend class Scene;
    friend class EntityBuilder;
    friend class Component;

public:
    // Enums

    // Public Fields

    static const Entity INVALID; /// @brief A constant representing an invalid/null entity.

    // Constructors and Destructors

    /// @brief Constructor with scene and handle.
    /// @param scene The scene that the entity belongs to.
    /// @param handle The EnTT entity handle to use.
    explicit Entity(Scene* const scene, const entt::entity handle);

    /// @brief Default destructor.
    ~Entity() = default;

    /// @brief Copy constructor.
    /// @param other The entity to copy from.
    inline Entity(const Entity& other)
        : _scene(other._scene), _handle(other._handle) {}

    /// @brief Creates a new entity in the specified scene.
    /// @param scene The scene where the entity will be created.
    /// @return A newly created EntityBuilder for configuring the entity.
    static EntityBuilder Create(Scene* const scene);

    // Public Methods

    /// @brief Copy assignment operator.
    /// @param other The entity to copy from.
    /// @return A reference to this entity after assignment.
    /// @note Uses const_cast to allow reassignment of the const handle member.
    inline Entity& operator=(const Entity& other) noexcept
    {
        if (this != &other) // Check for self-assignment
        {
            // Since handle is const to prevent accidental modification,
            // we need to use const_cast here to enable the desired
            // reassignment behavior
            *const_cast<entt::entity*>(&_handle) = other._handle;
            *const_cast<Scene**>(&_scene) = other._scene;
        }
        return *this;
    }

    /// @brief Equality comparison operator.
    /// @param other The entity to compare with.
    /// @return True if the entities have the same handle, false otherwise.
    inline bool operator==(const Entity& other) const { return _scene == other._scene && _handle == other._handle; }

    /// @brief Inequality comparison operator.
    /// @param other The entity to compare with.
    /// @return True if the entities have different handles, false otherwise.
    inline bool operator!=(const Entity& other) const { return _scene != other._scene && _handle != other._handle; }

    /// @brief Boolean conversion operator.
    /// @details Allows the entity to be used in conditional expressions.
    ///          An entity evaluates to true if it is valid, false otherwise.
    /// @return True if the entity is valid, false otherwise.
    inline explicit operator bool() const { return IsValid(); }

    /// @brief Gets the hash code for this entity.
    /// @return A hash value based on the entity's handle.
    /// @details Used for storing entities in hash-based containers.
    inline size_t GetHashCode() const { return std::hash<entt::entity>{}(_handle); }

    /// @brief Checks if the entity is valid.
    /// @return True if the entity's scene pointer is not null and its handle is valid in the registry, false otherwise.
    bool IsValid() const;

    /// @brief Marks an entity to be destroyed.
    /// @details This marks the entity for destruction rather than destroying it immediately.
    void MarkForDestruction();

    Scene* const GetScene() const { return _scene; }

    /// @brief Gets the name of this entity.
    /// @details Retrieves the name stored in the Name component of this entity.
    /// @return A string containing the entity's name.
    /// @pre Entity must have a Name component.
    const std::string& GetName() const;

    /// @brief Sets the name of this entity.
    /// @details Updates the name stored in the Name component of this entity.
    /// @param newName The new name to assign to this entity.
    /// @pre Entity must have a Name component.
    void SetName(const std::string& newName);

    /// @brief Gets the Transform component of this entity.
    /// @details This is a convenience method that assumes the entity has a Transform component.
    ///          This method should only be called when you know the entity has a Transform.
    /// @return A reference to the entity's Transform component.
    /// @pre Entity must have a Transform component.
    Transform& GetTransform() const;



    // ========== Tag Management ==========



    /// @brief Checks if an entity has a tag of the specified type.
    /// @tparam TagType The type of tag to check for. Must inherit from Tag.
    /// @return True if the entity has the specified tag, false otherwise.
    template<typename TagType, typename = IsTag<TagType>>
    bool HasTag() const
    {
        return _scene->HasTag<TagType>(*this);
    }

    /// @brief Attempts to add a tag of the specified type to an entity.
    /// @tparam TagType The type of tag to add. Must inherit from Tag.
    /// @return True if the tag was successfully added, false if the entity already had the tag.
    /// @details Safe to call even if the entity already has the specified tag.
    template<typename TagType, typename = IsTag<TagType>>
    bool TryAddTag()
    {
        return _scene->TryAddTag<TagType>(*this);
    }

    /// @brief Attempts to remove a tag of the specified type from an entity.
    /// @tparam TagType The type of tag to remove. Must inherit from Tag.
    /// @return True if the tag was successfully removed, false if the entity didn't have the tag.
    /// @details Safe to call even if the entity doesn't have the specified tag.
    template<typename TagType, typename = IsTag<TagType>>
    bool TryRemoveTag()
    {
        return _scene->TryRemoveTag<TagType>(*this);
    }



    // ========== Component Management ==========



    /// @brief Checks if an entity has a component of the specified type.
    /// @tparam ComponentType The type of component to check for. Must inherit from Component.
    /// @return True if the entity has the specified component, false otherwise.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool HasComponent() const
    {
        return _scene->HasComponent<ComponentType>(*this);
    }

    /// @brief Tries to get a mutable component of the specified type from the entity.
    /// @tparam ComponentType The type of component to get. Must inherit from Component.
    /// @param outComponent A pointer that will be set to the component if found, or nullptr if not found.
    /// @return True if the component was found, false otherwise.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool TryGetComponent(ComponentType*& outComponent)
    {
        return _scene->TryGetComponent(*this, outComponent);
    }

    /// @brief Tries to get a const component of the specified type from the entity.
    /// @tparam ComponentType The type of component to get. Must inherit from Component.
    /// @param outComponent A const pointer that will be set to the component if found, or nullptr if not found.
    /// @return True if the component was found, false otherwise.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool TryGetComponent(const ComponentType*& outComponent) const
    {
        return _scene->TryGetComponent(*this, outComponent);
    }

    /// @brief Attempts to add a component of the specified type to this entity.
    /// @tparam ComponentType The type of component to add. Must inherit from Component.
    /// @param outComponent A pointer that will be set to the component if successfully added, or nullptr if this entity already has the component.
    /// @return True if the component was successfully added, false if this entity already has the component.
    /// @details The component is default-constructed and its scene/handle are automatically set.
    ///          Safe to call even if this entity already has the specified component type.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool TryAddComponent(ComponentType*& outComponent)
    {
        return _scene->TryAddComponent<ComponentType>(*this, outComponent);
    }

    /// @brief Attempts to add a const component of the specified type to this entity.
    /// @tparam ComponentType The type of component to add. Must inherit from Component.
    /// @param outComponent A const pointer that will be set to the component if successfully added, or nullptr if this entity already has the component.
    /// @return True if the component was successfully added, false if this entity already has the component.
    /// @details The component is default-constructed and its scene/handle are automatically set.
    ///          This const overload provides read-only access to the newly added component.
    ///          Safe to call even if this entity already has the specified component type.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool TryAddComponent(const ComponentType*& outComponent)
    {
        return _scene->TryAddComponent<ComponentType>(*this, outComponent);
    }

    /// @brief Attempts to add a component of the specified type to this entity with constructor arguments.
    /// @tparam ComponentType The type of component to add. Must inherit from Component.
    /// @tparam Args The types of the constructor arguments.
    /// @param outComponent A pointer that will be set to the component if successfully added, or nullptr if this entity already has the component.
    /// @param args The constructor arguments to forward to the component constructor.
    /// @return True if the component was successfully added, false if this entity already has the component.
    /// @details The component is constructed with the provided arguments and its scene/handle are automatically set.
    ///          Safe to call even if this entity already has the specified component type.
    template<typename ComponentType, typename = IsComponent<ComponentType>, typename... Args>
    bool TryAddComponent(ComponentType*& outComponent, Args &&...args)
    {
        return _scene->TryAddComponent<ComponentType>(*this, outComponent, std::forward<Args>(args)...);
    }

    /// @brief Attempts to add a const component of the specified type to this entity with constructor arguments.
    /// @tparam ComponentType The type of component to add. Must inherit from Component.
    /// @tparam Args The types of the constructor arguments.
    /// @param outComponent A const pointer that will be set to the component if successfully added, or nullptr if this entity already has the component.
    /// @param args The constructor arguments to forward to the component constructor.
    /// @return True if the component was successfully added, false if this entity already has the component.
    /// @details The component is constructed with the provided arguments and its scene/handle are automatically set.
    ///          This const overload provides read-only access to the newly added component.
    ///          Safe to call even if this entity already has the specified component type.
    template<typename ComponentType, typename = IsComponent<ComponentType>, typename... Args>
    bool TryAddComponent(const ComponentType*& outComponent, Args &&...args)
    {
        return _scene->TryAddComponent<ComponentType>(*this, outComponent, std::forward<Args>(args)...);
    }

    /// @brief Attempts to remove a component of the specified type from this entity.
    /// @tparam ComponentType The type of component to remove. Must inherit from Component.
    /// @return True if the component was successfully removed, false if this entity didn't have the component.
    /// @details Safe to call even if this entity doesn't have the specified component type.
    template<typename ComponentType, typename = IsComponent<ComponentType>>
    bool TryRemoveComponent()
    {
        return _scene->TryRemoveComponent<ComponentType>(*this);
    }

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    Scene* const _scene;        ///< @brief Pointer to the scene this entity belongs to.
    const entt::entity _handle; ///< @brief The EnTT entity handle.

    // Private Methods

    /// @brief Default constructor.
    /// @details Creates an entity with an invalid handle. Used for creating the INVALID constant.
    inline explicit Entity()
        : _scene(nullptr), _handle(entt::null) {}
};

} // namespace velecs::ecs


/// @brief Standard library specialization for hashing Entity objects.
namespace std {

/// @brief Hash specialization for Entity objects.
/// @details Enables Entity objects to be used as keys in std::unordered_map and std::unordered_set.
template <>
struct hash<velecs::ecs::Entity> {
    /// @brief Computes hash value for an Entity.
    /// @param entity The entity to compute the hash for.
    /// @return Hash value for the entity.
    size_t operator()(const velecs::ecs::Entity& entity) const
    {
        return entity.GetHashCode();
    }
};

} // namespace std