#include "velecs/ecs/Entity.hpp"

namespace velecs::ecs {

// Constructors and Destructors

// Public Methods

// ========== Tag Management ==========

template<typename TagType, typename>
bool Scene::HasTag(const Entity* const entity) const
{
    return GetRegistry().all_of<TagType>(entity->_handle);
}

template<typename TagType, typename>
bool Scene::TryAddTag(Entity* const entity)
{
    if (HasTag<TagType>(entity)) return false;
    GetRegistry().emplace<TagType>(entity->_handle);
    return true;
}

template<typename TagType, typename>
bool Scene::TryRemoveTag(Entity* const entity)
{
    return GetRegistry().remove<TagType>(entity->_handle) > 0;
}



// ========== Component Management ==========



template<typename ComponentType, typename>
bool Scene::HasComponent(const Entity* const entity) const
{
    return GetRegistry().all_of<ComponentType>(entity->_handle);
}

template<typename ComponentType, typename>
bool Scene::TryGetComponent(const Entity* const entity, ComponentType*& outComponent)
{
    assert(entity && entity->IsValid() && "Entity must be valid");
    outComponent = GetRegistry().try_get<ComponentType>(entity->_handle);
    return (outComponent != nullptr);
}

template<typename ComponentType, typename>
bool Scene::TryGetComponent(const Entity* const entity, const ComponentType*& outComponent) const
{
    assert(entity && entity->IsValid() && "Entity must be valid");
    outComponent = GetRegistry().try_get<ComponentType>(entity->_handle);
    return (outComponent != nullptr);
}

template<typename ComponentType, typename>
bool Scene::TryAddComponent(Entity* const entity, ComponentType*& outComponent)
{
    assert(entity && entity->IsValid() && "Entity must be valid");
    if (HasComponent<ComponentType>(entity))
    {
        outComponent = nullptr;
        return false;
    }

    ComponentType& comp = GetRegistry().emplace<ComponentType>(entity->_handle);
    comp._owner = entity;
    outComponent = &comp;
    return true;
}

template<typename ComponentType, typename>
bool Scene::TryAddComponent(Entity* const entity, const ComponentType*& outComponent)
{
    assert(entity && entity->IsValid() && "Entity must be valid");
    if (HasComponent<ComponentType>(entity))
    {
        outComponent = nullptr;
        return false;
    }

    ComponentType& comp = GetRegistry().emplace<ComponentType>(entity->_handle);
    comp._owner = entity;
    outComponent = &comp;
    return true;
}

template<typename ComponentType, typename, typename... Args>
bool Scene::TryAddComponent(Entity* const entity, ComponentType*& outComponent, Args &&...args)
{
    assert(entity && entity->IsValid() && "Entity must be valid");
    if (HasComponent<ComponentType>(entity))
    {
        outComponent = nullptr;
        return false;
    }

    ComponentType& comp = GetRegistry().emplace<ComponentType>(entity->_handle, std::forward<Args>(args)...);
    comp._owner = entity;
    outComponent = &comp;
    return true;
}

template<typename ComponentType, typename, typename... Args>
bool Scene::TryAddComponent(Entity* const entity, const ComponentType*& outComponent, Args &&...args)
{
    assert(entity && entity->IsValid() && "Entity must be valid");
    if (HasComponent<ComponentType>(entity))
    {
        outComponent = nullptr;
        return false;
    }

    ComponentType& comp = GetRegistry().emplace<ComponentType>(entity->_handle, std::forward<Args>(args)...);
    comp._owner = entity;
    outComponent = &comp;
    return true;
}

template<typename ComponentType, typename>
bool Scene::TryRemoveComponent(Entity* const entity)
{
    assert(entity && entity->IsValid() && "Entity must be valid");
    if (!HasComponent<ComponentType>(entity)) return false;
    GetRegistry().remove<ComponentType>(entity->_handle);
    return true;
}

// Protected Methods

// Private Methods

} // namespace velecs::ecs
