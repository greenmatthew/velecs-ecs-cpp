/// @file    Scene.cpp
/// @author  Matthew Green
/// @date    2025-07-27 19:13:20
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#include "velecs/ecs/Scene.hpp"

#include "velecs/ecs/Entity.hpp"
#include "velecs/ecs/EntityBuilder.hpp"
#include "velecs/ecs/Component.hpp"

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

Scene::Scene(const std::string& name, ConstructorKey)
    : _name(name) {}

Scene::~Scene() = default;

// Public Methods

bool Scene::IsEntityHandleValid(const Entity entity)
{
    return entity._handle != entt::null
        && _registry.valid(entity._handle);
}

EntityBuilder Scene::CreateEntity()
{
    return EntityBuilder(this, _registry.create());
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

void Scene::DestroyEntity(Entity entity)
{
    _registry.destroy(entity._handle);
}

void Scene::ProcessCleanup()
{
    // deque? queue? vector? idk
    std::vector<Entity> destructionQueue;
    _registry.view<DestroyTag>().each([&](auto e, auto& tag){
        Entity entity{this, e};
        auto& transform = entity.GetTransform();
        destructionQueue.push_back(entity);
    });

    
    for (Entity entityToDelete : destructionQueue)
    {
        // May already have been deleted
        if (!entityToDelete.IsValid()) continue;

        for (auto [entity, transform] : entityToDelete.GetTransform().Traverse<TraversalOrder::PostOrder>())
        {
            // Do not forget to remove the root entity from its parent's children vector
            if (entity == entityToDelete) transform.TrySetParent(Entity::INVALID);

            DestroyEntity(entity);
        }
    }
}

} // namespace velecs::ecs
