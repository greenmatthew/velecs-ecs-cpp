/// @file    Entity.cpp
/// @author  Matthew Green
/// @date    2025-05-12 16:24:39
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#include "velecs/ecs/Entity.hpp"
#include "velecs/ecs/internal/EntityRegistry.hpp"
#include "velecs/ecs/internal/EntityPersistence.hpp"

#include <memory>

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

Entity::Entity(entt::registry& registry, entt::entity handle)
    : registry(registry), handle(handle), name("Entity") {}

// Public Methods

EntityRef Entity::Create()
{
    entt::registry& registry = EntityRegistry::GetRegistry();
    entt::entity handle =  registry.create();

    auto entityPtr = std::make_unique<Entity>(registry, handle);

    // Store the unique_ptr in the persistence component
    auto& persistence = registry.emplace<EntityPersistence>(handle, std::move(entityPtr));

    return persistence.CreateEntityRef();
}

void Entity::RequestDestroy(EntityRef entityRef)
{
    if (entityRef.IsAlive())
    {
        auto& persistence = entityRef->registry.get<EntityPersistence>(entityRef->handle);
        auto ptr = persistence.Transfer(); // transfers ownership of std::unique_ptr<Entity>
        destructionQueue.push_back(std::move(ptr));
    }
}

void Entity::ProcessDestructionQueue()
{
    for (auto& entityPtr : destructionQueue)
    {
        entityPtr->Destroy();
    }
    
    // Clear the queue (this will destroy all unique_ptrs and release their memory)
    destructionQueue.clear();
}

// Protected Fields

// Protected Methods

// Private Fields

std::vector<std::unique_ptr<Entity>> Entity::destructionQueue = std::vector<std::unique_ptr<Entity>>(10);

// Private Methods

void Entity::Destroy()
{
    registry.destroy(handle);
    delete this;
}

} // namespace velecs::ecs
