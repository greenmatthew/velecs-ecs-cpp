/// @file    World.hpp
/// @author  Matthew Green
/// @date    2025-08-12 11:13:10
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include "velecs/ecs/Object.hpp"
#include "velecs/ecs/SceneManager.hpp"

#include <velecs/common/Uuid.hpp>
using velecs::common::Uuid;

#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <cassert>

namespace velecs::ecs {

/// @class World
/// @brief Manages the lifecycle and storage of all objects in the ECS system.
/// @details Provides type-safe storage and retrieval of objects organized by type.
///          Uses std::type_index for efficient type-based lookups.
class World {
public:
    // Enums

    // Public Fields

    using ObjectStorage = std::unique_ptr<Object>;
    using ObjectMap = std::unordered_map<Uuid, ObjectStorage>;
    using TypedObjectStorageMap = std::unordered_map<std::type_index, ObjectMap>;

    std::unique_ptr<SceneManager> scenes{nullptr};

    // Constructors and Destructors

    /// @brief Default constructor.
    inline World()
        : scenes(std::make_unique<SceneManager>(this)) {}

    /// @brief Default deconstructor.
    ~World() = default;

    // Delete copy operations to ensure single ownership
    World(const World&) = delete;
    World& operator=(const World&) = delete;

    // Allow move operations
    World(World&&) = default;
    World& operator=(World&&) = default;

    // Public Methods

    /// @brief Registers an object with the manager.
    /// @tparam ObjectT The type of object being registered.
    /// @param obj Unique pointer to the object to register.
    /// @details Stores the object in a type-specific map using its UUID as the key.
    ///          This should generally never fail unless there's a UUID collision (extremely rare)
    ///          or double registration (programming error).
    template<typename ObjectT>
    void Internal_Register(std::unique_ptr<ObjectT> obj)
    {
        static_assert(std::is_base_of_v<Object, ObjectT>, "ObjectT must inherit from Object");
        
        if (!obj)
        {
            assert(false && "Cannot register null object");
            return;
        }

        std::type_index typeKey = std::type_index(typeid(ObjectT));
        Uuid uuid = obj->GetUuid();
        
        // Get or create the type map
        auto& typeMap = _objects[typeKey];
        
        // Attempt to insert the object
        auto [it, inserted] = typeMap.emplace(uuid, std::move(obj));
        
        assert(inserted && "Object with this UUID already exists! This indicates either "
                          "a double registration or UUID collision (both should never happen).");
    }

    /// @brief Attempts to retrieve an object by type and UUID.
    /// @tparam ObjectT The type of object to retrieve.
    /// @param uuid The UUID of the object to find.
    /// @return Pointer to the object if found, nullptr otherwise.
    template<typename ObjectT>
    ObjectT* TryGet(const Uuid& uuid)
    {
        static_assert(std::is_base_of_v<Object, ObjectT>, "ObjectT must inherit from Object");
        
        std::type_index typeKey = std::type_index(typeid(ObjectT));
        
        auto typeIt = _objects.find(typeKey);
        if (typeIt == _objects.end()) return nullptr;
        
        auto objIt = typeIt->second.find(uuid);
        if (objIt == typeIt->second.end()) return nullptr;
        
        return static_cast<ObjectT*>(objIt->second.get());
    }

    /// @brief Attempts to remove an object by type and UUID.
    /// @tparam ObjectT The type of object to remove.
    /// @param uuid The UUID of the object to remove.
    /// @return True if the object was found and removed, false otherwise.
    template<typename ObjectT>
    bool TryRemove(const Uuid& uuid)
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

    /// @brief Gets the number of objects of a specific type.
    /// @tparam ObjectT The type of object to count.
    /// @return Number of objects of the specified type.
    template<typename ObjectT>
    size_t GetCount() const
    {
        static_assert(std::is_base_of_v<Object, ObjectT>, "ObjectT must inherit from Object");
        
        std::type_index typeKey = std::type_index(typeid(ObjectT));
        auto typeIt = _objects.find(typeKey);
        return (typeIt != _objects.end()) ? typeIt->second.size() : 0;
    }

    /// @brief Gets the total number of objects across all types.
    /// @return Total number of managed objects.
    size_t GetTotalCount() const;

    /// @brief Checks if any objects of the specified type exist.
    /// @tparam ObjectT The type to check for.
    /// @return True if any objects of this type exist, false otherwise.
    template<typename ObjectT>
    bool HasAny() const
    {
        return GetCount<ObjectT>() > 0;
    }

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    TypedObjectStorageMap _objects; ///< @brief Nested map: type -> (UUID -> object)

    // Private Methods
};

} // namespace velecs::ecs
