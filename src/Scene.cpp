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
#include "velecs/ecs/System.hpp"

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

Scene::Scene(const std::string& name, size_t systemCapacity, ConstructorKey)
    : _name(name)
{
    _systemsIterator.reserve(systemCapacity);
    _systems.reserve(systemCapacity);
}

Scene::Scene(const std::string& name, ConstructorKey key)
    : Scene(name, DEFAULT_SYSTEM_CAPACITY, key) {}

Scene::~Scene() = default;

// Public Methods

bool Scene::IsEntityHandleValid(const Entity entity)
{
    return entity._handle != entt::null
        && GetRegistry().valid(entity._handle);
}

EntityBuilder Scene::CreateEntity()
{
    return EntityBuilder(this, GetRegistry().create());
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

entt::registry& Scene::GetRegistry()
{
    if (_registry.has_value()) return *_registry;
    throw std::runtime_error("Scene does not have a valid registry initialized");
}

void Scene::Init()
{
    _registry.emplace();
    OnEnter();
}

void Scene::Cleanup()
{
    if (_registry.has_value())
    {
        OnExit();
        _registry->clear();
        _registry.reset();
    }
}

void Scene::DestroyEntity(Entity entity)
{
    GetRegistry().destroy(entity._handle);
}

void Scene::Process(void* context)
{
    for (auto id : _systemsIterator)
    {
        System* system = _systems[id].get();
        if (!system->IsEnabled()) continue;
        system->Process(context);
    }
}

void Scene::ProcessPhysics(void* context)
{
    for (auto id : _systemsIterator)
    {
        System* system = _systems[id].get();
        if (!system->IsEnabled()) continue;
        system->ProcessPhysics(context);
    }
}

void Scene::ProcessGUI(void* context)
{
    for (auto id : _systemsIterator)
    {
        System* system = _systems[id].get();
        if (!system->IsEnabled()) continue;
        system->ProcessGUI(context);
    }
}

void Scene::ProcessEntityCleanup()
{
    // deque? queue? vector? idk
    std::vector<Entity> destructionQueue;
    GetRegistry().view<DestroyTag>().each([&](auto e, auto& tag){
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
