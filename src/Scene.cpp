/// @file    Scene.cpp
/// @author  Matthew Green
/// @date    2025-07-27 19:13:20
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#include "velecs/ecs/Component.hpp"
#include "velecs/ecs/Entity.hpp"
#include "velecs/ecs/EntityBuilder.hpp"
#include "velecs/ecs/Scene.hpp"
#include "velecs/ecs/System.hpp"
#include "velecs/ecs/World.hpp"

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

Scene::Scene(World* const world, const std::string& name, size_t systemCapacity)
    : Object(world, name)
{
    _systemsIterator.reserve(systemCapacity);
    _systems.reserve(systemCapacity);
}

Scene::~Scene() = default;

// Public Methods

bool Scene::IsEntityHandleValid(const Entity* const entity)
{
    return entity != nullptr
        && entity->_scene == this
        && entity->GetWorld() == GetWorld()
        && GetRegistry().valid(entity->_handle);
}

EntityBuilder Scene::CreateEntity()
{
    Entity* const entity = Object::Create<Entity>(GetWorld(), this, GetRegistry().create());
    Uuid uuid = entity->GetUuid();
    auto handle = entity->_handle;
    auto [it, inserted] = _entities.try_emplace(handle, uuid);
    assert(inserted && "A new entity should never fail to be inserted");
    return EntityBuilder(entity);
}

// Protected Fields

// Protected Methods

Entity* Scene::TryGetEntity(const entt::entity handle)
{
    auto it = _entities.find(handle);
    if (it == _entities.end()) return nullptr;
    auto uuid = it->second;
    return GetWorld()->TryGet<Entity>(uuid);
}

// Private Fields

// Private Methods

entt::registry& Scene::GetRegistry()
{
    if (_registry.has_value()) return *_registry;
    throw std::runtime_error("Scene does not have a valid registry initialized");
}

const entt::registry& Scene::GetRegistry() const
{
    if (_registry.has_value()) return *_registry;
    throw std::runtime_error("Scene does not have a valid registry initialized");
}

void Scene::Init(void* context)
{
    // Instantiates the scene's EnTT registry
    _registry.emplace();
    std::cout << "Registry emplaced for: " << GetName() << std::endl;
    OnEnter(context);
}

void Scene::Cleanup(void* context)
{
    if (_registry.has_value())
    {
        OnExit(context);
        // Released the scene's EnTT registry
        _registry->clear();
        _registry.reset();
    }
}

void Scene::DestroyEntity(Entity* const entity)
{
    if (!entity || !entity->IsValid()) return;
    GetRegistry().destroy(entity->_handle);
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
    std::vector<Entity*> destructionQueue;
    GetRegistry().view<DestroyTag>().each([&](auto e, auto& tag){
        Entity* entity = TryGetEntity(e);
        assert(entity && "Should always be able to lookup entity via entt handle");
        destructionQueue.push_back(entity);
    });

    
    for (Entity* entityToDelete : destructionQueue)
    {
        // May already have been deleted
        if (!entityToDelete->IsValid()) continue;

        for (auto [entity, transform] : entityToDelete->GetTransform().Traverse<TraversalOrder::PostOrder>())
        {
            // Do not forget to remove the root entity from its parent's children vector
            if (entity == entityToDelete) transform.TrySetParent(nullptr);

            DestroyEntity(entity);
        }
    }
}

} // namespace velecs::ecs
