#pragma once

#include "velecs/ecs/Scene.hpp"

namespace velecs::ecs {

// Public Methods

// ========== Tag Management ==========

template<typename TagType, typename>
bool Entity::HasTag() const
{
    return _scene->template HasTag<TagType>(this);
}

template<typename TagType, typename>
bool Entity::TryAddTag()
{
    return _scene->template TryAddTag<TagType>(this);
}

template<typename TagType, typename>
bool Entity::TryRemoveTag()
{
    return _scene->template TryRemoveTag<TagType>(this);
}



// ========== Component Management ==========



template<typename ComponentType, typename>
bool Entity::HasComponent() const
{
    return _scene->template HasComponent<ComponentType>(this);
}

template<typename ComponentType, typename>
bool Entity::TryGetComponent(ComponentType*& outComponent)
{
    return _scene->template TryGetComponent(this, outComponent);
}

template<typename ComponentType, typename>
bool Entity::TryGetComponent(const ComponentType*& outComponent) const
{
    return _scene->TryGetComponent(this, outComponent);
}

template<typename ComponentType, typename>
bool Entity::TryAddComponent(ComponentType*& outComponent)
{
    return _scene->template TryAddComponent<ComponentType>(this, outComponent);
}

template<typename ComponentType, typename>
bool Entity::TryAddComponent(const ComponentType*& outComponent)
{
    return _scene->template TryAddComponent<ComponentType>(this, outComponent);
}

template<typename ComponentType, typename, typename... Args>
bool Entity::TryAddComponent(ComponentType*& outComponent, Args &&...args)
{
    return _scene->template TryAddComponent<ComponentType>(this, outComponent, std::forward<Args>(args)...);
}

template<typename ComponentType, typename, typename... Args>
bool Entity::TryAddComponent(const ComponentType*& outComponent, Args &&...args)
{
    return _scene->template TryAddComponent<ComponentType>(this, outComponent, std::forward<Args>(args)...);
}

template<typename ComponentType, typename>
bool Entity::TryRemoveComponent()
{
    return _scene->template TryRemoveComponent<ComponentType>(this);
}

// Protected Methods

// Private Methods

} // namespace velecs::ecs
