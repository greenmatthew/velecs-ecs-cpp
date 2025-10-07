#include "velecs/ecs/Object.hpp"
#include "velecs/ecs/SceneManager.hpp"

namespace velecs::ecs {

// Constructors and Destructors

World::World()
    : scenes(std::make_unique<SceneManager>(this)) {}

// Public Methods

template<typename ObjectT, typename... Args>
ObjectT* World::Create(Args&&... args)
{
    static_assert(std::is_base_of_v<Object, ObjectT>, "ObjectT must inherit from Object");
    
    auto objStorage = std::make_unique<ObjectT>(std::forward<Args>(args)...);
    objStorage->_uuid = Uuid::GenerateRandom();
    auto* obj = objStorage.get();
    Register<ObjectT>(std::move(objStorage));
    
    return obj;
}

template<typename StorageT, typename ObjectT, typename... Args>
ObjectT* World::CreateAs(Args&&... args)
{
    static_assert(std::is_base_of_v<Object, ObjectT>, "ObjectT must inherit from Object");
    static_assert(std::is_base_of_v<StorageT, ObjectT>, "ObjectT must inherit from StorageT");
    
    auto objStorage = std::make_unique<ObjectT>(std::forward<Args>(args)...);
    objStorage->_uuid = Uuid::GenerateRandom();
    auto* obj = objStorage.get();
    RegisterAs<ObjectT, StorageT>(std::move(objStorage));
    
    return obj;
}

template<typename ObjectT>
ObjectT* World::TryGet(const Uuid& uuid)
{
    static_assert(std::is_base_of_v<Object, ObjectT>, "ObjectT must inherit from Object");
    
    std::type_index typeKey = std::type_index(typeid(ObjectT));
    
    auto typeIt = _objects.find(typeKey);
    if (typeIt == _objects.end()) return nullptr;
    
    auto objIt = typeIt->second.find(uuid);
    if (objIt == typeIt->second.end()) return nullptr;
    
    return static_cast<ObjectT*>(objIt->second.get());
}

template<typename ObjectT>
std::vector<ObjectT*> World::TryGet(const std::string& name)
{
    static_assert(std::is_base_of_v<Object, ObjectT>, "ObjectT must inherit from Object");
    
    std::type_index typeKey = std::type_index(typeid(ObjectT));

    std::vector<ObjectT*> objs;
    
    auto typeIt = _objects.find(typeKey);
    if (typeIt == _objects.end()) return objs;

    for (const auto& [uuid, objStorage] : typeIt->second)
    {
        auto obj = objStorage.get();
        if (obj->GetName() == name) objs.push_back(static_cast<ObjectT*>(obj));
    }

    return objs;
}

template<typename ObjectT>
bool World::TryRemove(const Uuid& uuid)
{
    static_assert(std::is_base_of_v<Object, ObjectT>, "ObjectT must inherit from Object");
    
    std::type_index typeKey = std::type_index(typeid(ObjectT));
    
    auto typeIt = _objects.find(typeKey);
    if (typeIt == _objects.end()) return false;
    
    auto objIt = typeIt->second.find(uuid);
    if (objIt == typeIt->second.end()) return false;
    
    typeIt->second.erase(objIt);
    
    // Clean up empty type maps to avoid memory bloat
    if (typeIt->second.empty()) {
        _objects.erase(typeIt);
    }
    
    return true;
}

template<typename ObjectT>
size_t World::GetCount() const
{
    static_assert(std::is_base_of_v<Object, ObjectT>, "ObjectT must inherit from Object");
    
    std::type_index typeKey = std::type_index(typeid(ObjectT));
    auto typeIt = _objects.find(typeKey);
    return (typeIt != _objects.end()) ? typeIt->second.size() : 0;
}


template<typename ObjectT>
bool World::HasAny() const
{
    return GetCount<ObjectT>() > 0;
}

// Protected Methods

// Private Methods

template<typename ObjectT>
void World::Register(std::unique_ptr<ObjectT> obj)
{
    static_assert(std::is_base_of_v<Object, ObjectT>, "ObjectT must inherit from Object");
    
    if (!obj) {
        assert(false && "Cannot register null object");
        return;
    }

    std::type_index typeKey = std::type_index(typeid(ObjectT));
    Uuid uuid = obj->GetUuid();
    
    auto& typeMap = _objects[typeKey];
    auto [it, inserted] = typeMap.emplace(uuid, std::move(obj));
    
    assert(inserted && "Object with this UUID already exists!");
}

template<typename ObjectT, typename StorageT>
void World::RegisterAs(std::unique_ptr<ObjectT> obj)
{
    static_assert(std::is_base_of_v<Object, ObjectT>, "ObjectT must inherit from Object");
    static_assert(std::is_base_of_v<StorageT, ObjectT>, "ObjectT must inherit from StorageT");
    
    if (!obj) {
        assert(false && "Cannot register null object");
        return;
    }

    std::type_index typeKey = std::type_index(typeid(StorageT));
    Uuid uuid = obj->GetUuid();
    
    auto& typeMap = _objects[typeKey];
    auto [it, inserted] = typeMap.emplace(uuid, std::move(obj));
    
    assert(inserted && "Object with this UUID already exists!");
}

} // namespace velecs::ecs
