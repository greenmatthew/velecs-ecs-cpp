#include "velecs/ecs/Entity.hpp"
#include "velecs/ecs/System.hpp"

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

// ========== System Management ==========



template<typename SystemType, typename>
bool Scene::HasSystem()
{
    SystemId id = GetSystemId<SystemType>();
    
    bool inMap = _systems.find(id) != _systems.end();
    auto iteratorIt = std::find(_systemsIterator.begin(), _systemsIterator.end(), id);
    bool inIterator = (iteratorIt != _systemsIterator.end());
    
    // Ensure both collections are synchronized
    assert(inMap == inIterator && 
        "System storage collections out of sync - system found in one collection but not the other");
    
    return inMap;
}

template<typename SystemType, typename>
bool Scene::TryAddSystem()
{
    return TryAddSystem(std::make_unique<SystemType>());
}

template<typename SystemType, typename, typename... Args>
bool Scene::TryAddSystem(Args&&... args)
{
    return TryAddSystem(std::make_unique<SystemType>(std::forward<Args>(args)...));
}

template<typename SystemType, typename>
bool Scene::TryRemoveSystem()
{
    SystemId id = GetSystemId<SystemType>();

    auto it = _systems.find(id);
    if (it == _systems.end()) return false;
    
    // Call cleanup before removal
    it->second->Cleanup();

    // Remove from map
    _systems.erase(it);
    
    // Find and remove from iterator vector (simple linear search)
    auto iteratorIt = std::find(_systemsIterator.begin(), _systemsIterator.end(), id);
    assert(iteratorIt != _systemsIterator.end() && *iteratorIt == id && 
            "System found in _systems map but not in _systemsIterator - data structure consistency violated");
    
    if (iteratorIt != _systemsIterator.end()) _systemsIterator.erase(iteratorIt);
    
    return true;
}

template<typename SystemType, typename>
Scene::SystemId Scene::GetSystemId()
{
    return typeid(SystemType);
}

template<typename TagOrComponent, typename Func>
void Scene::Query(Func&& callback)
{
    auto& registry = GetRegistry();
    registry.view<Component>().each([this, &callback](entt::entity e, TagOrComponent& tagOrComp) {
        Entity* entity = TryGetEntity(e);
        assert(entity && "Should always be able to lookup entity via entt handle");
        callback(entity, tagOrComp);
    });
}

template<typename... TagsOrComponents, typename Func>
void Scene::Query(Func&& callback)
{
    static_assert(sizeof...(TagsOrComponents) > 1, "Use single component Query overload for single component queries");
    
    GetRegistry().view<TagsOrComponents...>().each([this, callback = std::forward<Func>(callback)](entt::entity e, TagsOrComponents&... tagsOrComps) {
        Entity* entity = TryGetEntity(e);
        assert(entity && "Should always be able to lookup entity via entt handle");
        callback(entity, tagsOrComps...);
    });
}

template<typename TagOrComponent>
void Scene::Query(std::function<void(Entity*, TagOrComponent&)> callback)
{
    auto& registry = GetRegistry();
    auto view = registry.view<TagOrComponent>();
    
    for (auto e : view) {
        Entity* entity = TryGetEntity(e);
        assert(entity && "Should always be able to lookup entity via entt handle");
        TagOrComponent& tagOrComp = registry.get<TagOrComponent>(entity->_handle);
        callback(entity, tagOrComp);
    }
}

template<typename... TagsOrComponents>
void Scene::Query(std::function<void(Entity*, TagsOrComponents&...)> callback)
{
    static_assert(sizeof...(TagsOrComponents) > 1, "Use single component Query overload for single component queries");
    
    GetRegistry().view<TagsOrComponents...>().each([this, &callback](entt::entity e, TagsOrComponents&... tagsOrComps) {
        Entity* entity = TryGetEntity(e);
        assert(entity && "Should always be able to lookup entity via entt handle");
        callback(entity, tagsOrComps...);
    });
}

// Protected Methods

// Private Methods

template<typename SystemType, typename>
bool Scene::TryAddSystem(std::unique_ptr<SystemType> system)
{
    SystemId id = GetSystemId<SystemType>();
    
    if (_systems.find(id) != _systems.end()) return false;
    
    auto [it, inserted] = _systems.emplace(id, std::move(system));
    assert(inserted && "System emplace failed after find() check");

    // Sorted insertion using execution order
    auto insertPos = std::lower_bound(_systemsIterator.begin(), _systemsIterator.end(), id,
        [this](const SystemId& a, const SystemId& b) {
            return _systems.at(a)->GetExecutionOrder() < _systems.at(b)->GetExecutionOrder();
        });
    _systemsIterator.insert(insertPos, id);

    it->second->Init();
    return inserted;
}

} // namespace velecs::ecs
