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

#include <iostream>

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

    std::string name;

    // Constructors and Destructors

    /// @brief Default constructor.
    inline explicit Entity()
        : handle(entt::null) {}

    /// @brief Default deconstructor.
    ~Entity() = default;

    // Public Methods

    static Entity Create();

    inline bool IsValid() const
    {
        return registry.valid(handle);
    }

    template<typename T, typename = IsComponent<T>>
    inline T& AddComponent()
    {
        return registry.emplace<T>(handle);
    }

    template<typename T, typename = IsComponent<T>, typename... Args>
    inline T& AddComponent(Args &&...args)
    {
        return registry.emplace<T>(handle, std::forward<Args>(args)...);
    }

    template<typename T, typename = IsComponent<T>>
    inline void RemoveComponent()
    {
        registry.remove<T>(handle);
    }

    template<typename T, typename = IsComponent<T>>
    inline bool TryGetComponent(T*& outComponent)
    {
        outComponent = registry.try_get<T>(handle);
        return (outComponent != nullptr);
    }

    template<typename T, typename = IsComponent<T>>
    inline bool TryGetComponent(const T*& outComponent)
    {
        outComponent = registry.try_get<T>(handle);
        return (outComponent != nullptr);
    }

    static void RequestDestroy(Entity entity);

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

    inline explicit Entity(entt::entity handle)
        : handle(handle) {}

    void Destroy();
};

} // namespace velecs::ecs
