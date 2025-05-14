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
#include "velecs/ecs/Component.hpp"

#include <iostream>
#include <functional>

#include <entt/entt.hpp>

namespace velecs::ecs {

/// @class Entity
/// @brief Brief description.
///
/// Rest of description.
class Entity {
public:
    // Enums

    // Public Fields

    static const Entity INVALID;

    // Constructors and Destructors

    /// @brief Default deconstructor.
    ~Entity() = default;

    // Public Methods

    /// @brief Creates a new entity in the registry.
    /// @return A newly created entity with a valid handle.
    static Entity Create();

    /// @brief Copy assignment operator.
    /// @param other The entity to copy from.
    /// @return A reference to this entity after assignment.
    inline Entity& operator=(const Entity& other)
    {
        if (this != &other) // Check for self-assignment
        {
            const_cast<entt::entity&>(handle) = other.handle; // Copy handle
        }
        return *this;
    }

    /// @brief Equality comparison operator.
    /// @param other The entity to compare with.
    /// @return True if the entities have the same handle, false otherwise.
    inline bool operator==(const Entity& other) const
    {
        return handle == other.handle;
    }

    /// @brief Inequality comparison operator.
    /// @param other The entity to compare with.
    /// @return True if the entities have different handles, false otherwise.
    inline bool operator!=(const Entity& other) const
    {
        return handle != other.handle;
    }

    /// @brief Checks if the entity is valid.
    /// @return True if the entity handle is valid in the registry, false otherwise.
    inline bool IsValid() const
    {
        return registry.valid(handle);
    }

    /// @brief Adds a component of type T to the entity.
    /// @tparam T The type of component to add.
    /// @return A reference to the newly added component.
    template<typename T, typename = IsComponent<T>>
    inline T& AddComponent()
    {
        T& comp = registry.emplace<T>(handle);
        comp.SetOwner(*this);
        return comp;
    }

    /// @brief Adds a component of type T to the entity with constructor arguments.
    /// @tparam T The type of component to add.
    /// @tparam Args The types of the constructor arguments.
    /// @param args The constructor arguments.
    /// @return A reference to the newly added component.
    template<typename T, typename = IsComponent<T>, typename... Args>
    inline T& AddComponent(Args &&...args)
    {
        T& comp = registry.emplace<T>(handle, std::forward<Args>(args)...);
        comp.SetOwner(*this);
        return comp;
    }

    /// @brief Removes a component of type T from the entity.
    /// @tparam T The type of component to remove.
    template<typename T, typename = IsComponent<T>>
    inline void RemoveComponent()
    {
        registry.remove<T>(handle);
    }

    /// @brief Tries to get a component of type T from the entity.
    /// @tparam T The type of component to get.
    /// @param outComponent A pointer that will be set to the component if found.
    /// @return True if the component was found, false otherwise.
    template<typename T, typename = IsComponent<T>>
    inline bool TryGetComponent(T*& outComponent)
    {
        outComponent = registry.try_get<T>(handle);
        return (outComponent != nullptr);
    }

    /// @brief Tries to get a const component of type T from the entity.
    /// @tparam T The type of component to get.
    /// @param outComponent A const pointer that will be set to the component if found.
    /// @return True if the component was found, false otherwise.
    template<typename T, typename = IsComponent<T>>
    inline bool TryGetComponent(const T*& outComponent)
    {
        outComponent = registry.try_get<T>(handle);
        return (outComponent != nullptr);
    }

    size_t GetHashCode() const
    {
        return std::hash<entt::entity>{}(handle);
    }

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

    static entt::registry registry;
    static std::vector<Entity> destructionQueue;
    
    const entt::entity handle;

    // Private Methods

    /// @brief Default constructor.
    /// @details Creates an entity with an invalid handle.
    inline explicit Entity()
        : handle(entt::null) {}

    /// @brief Constructor with handle.
    /// @param handle The entity handle to use.
    inline explicit Entity(entt::entity handle)
        : handle(handle) {}

    /// @brief Destroys this entity.
    /// @details Removes the entity from the registry.
    void Destroy();
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