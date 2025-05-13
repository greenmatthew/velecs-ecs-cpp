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
#include "velecs/ecs/EntityRef.hpp"

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

    std::string name;

    // Constructors and Destructors

    /// @brief Disable default constructor.
    Entity() = delete;

    /// @brief Default constructor.
    Entity(entt::registry& registry, entt::entity handle);

    /// @brief Default deconstructor.
    // ~Entity() = default;
    ~Entity()
    {
        std::cout << "Entity '" << name << "' destroyed" << std::endl;
    }

    // Public Methods

    static EntityRef Create();

    inline bool IsValid() const
    {
        return registry.valid(handle);
    }

    inline static bool IsAlive(const Entity* entity)
    {
        return entity != nullptr && entity->IsValid();
    }

    template<typename T, typename = IsComponent<T>>
    inline T& AddComponent()
    {
        return registry.emplace<T>(handle);
    }

    // template<typename T, IsComponent<T>, typename... Args>
    // void AddComponent(Args &&...args)
    // {
    //     auto& comp = registry.emplace<T>(handle, std::forward<Args>(args)...);
    //     // return comp;
    // }

    template<typename T, typename = IsComponent<T>>
    void RemoveComponent();

    template<typename T, typename = IsComponent<T>>
    void TryGetComponent();

    static void RequestDestroy(EntityRef entityRef);

    static void ProcessDestructionQueue();

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    static std::vector<std::unique_ptr<Entity>> destructionQueue;

    entt::registry& registry;
    const entt::entity handle;

    // Private Methods

    /// @brief Copy constructor disabled externally
    Entity(const Entity&) = default;

    /// @brief Assignment operator disabled externally
    Entity& operator=(const Entity&) = default;

    void Destroy();
};

} // namespace velecs::ecs
