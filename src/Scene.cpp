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

bool Scene::IsEntityValid(const Entity entity)
{
    return entity._handle != entt::null
        && _registry.valid(entity._handle);
}

EntityBuilder Scene::CreateEntity()
{
    return EntityBuilder(this, _registry.create());
}

void Scene::ProcessCleanup()
{
    // if (_markedView == nullptr)
    // {
    //     *_markedView = _registry.view<DestroyTag>();
    // }

    // // deque? queue? vector? idk
    // std::deque<Entity> destructionQueue;
    // _markedView->each([&](auto e){
    //     Entity entity{this, e}; // Pass in Scene ptr and entt::entity handle
    //     auto& transform = entity.GetTransform();
    //     // Traverse transform to children then up... whatever that traversal is call
    //     // then add child to destruction queue
    //     destructionQueue.push_front(entity);
    // });

    // for (Entity entity : destructionQueue)
    // {

    //     // Only need to update parent of root entity.... not sure how to handle that... maybe we do it in the view.each instead...
    //     // Remove the root entities from there parent there...
    //     // Then safely destroy everything here without a notifyParent flag....
    //     if (entity == destructionQueue.end()) entity.DestroyImmediate(notifyParent: true);
    //     else entity.DestroyImmediate(notifyParent: false);
    //     _registry.destroy(entity._handle);
    // }
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

} // namespace velecs::ecs
