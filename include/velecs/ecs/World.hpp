#pragma once

#include <velecs/common/Uuid.hpp>
using velecs::common::Uuid;

#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <cassert>

namespace velecs::ecs {

class Object;
class SceneManager;

/// @class World
/// @brief Manages the lifecycle and storage of all objects in the ECS system.
/// @details Provides type-safe storage and retrieval of objects organized by type.
///          Uses std::type_index for efficient type-based lookups.
class World {
public:
    // Public Fields

    using ObjectStorage = std::unique_ptr<Object>;
    using ObjectMap = std::unordered_map<Uuid, ObjectStorage>;
    using TypedObjectStorageMap = std::unordered_map<std::type_index, ObjectMap>;

    std::unique_ptr<SceneManager> scenes{nullptr};

    // Constructors and Destructors

    /// @brief Default constructor.
    inline World();

    /// @brief Default deconstructor.
    ~World() = default;

    // Delete copy operations to ensure single ownership
    World(const World&) = delete;
    World& operator=(const World&) = delete;

    // Allow move operations
    World(World&&) = default;
    World& operator=(World&&) = default;

    // Public Methods

    template<typename ObjectT, typename... Args>
    ObjectT* Create(Args&&... args);

    template<typename StorageT, typename ObjectT, typename... Args>
    ObjectT* CreateAs(Args&&... args);

    /// @brief Attempts to retrieve an object by type and UUID.
    /// @tparam ObjectT The type of object to retrieve.
    /// @param uuid The UUID of the object to find.
    /// @return Pointer to the object if found, nullptr otherwise.
    template<typename ObjectT>
    ObjectT* TryGet(const Uuid& uuid);

    /// @brief Attempts to retrieve an object by type and UUID.
    /// @tparam ObjectT The type of object to retrieve.
    /// @param uuid The UUID of the object to find.
    /// @return Pointer to the object if found, nullptr otherwise.
    template<typename ObjectT>
    std::vector<ObjectT*> TryGet(const std::string& name);

    /// @brief Attempts to remove an object by type and UUID.
    /// @tparam ObjectT The type of object to remove.
    /// @param uuid The UUID of the object to remove.
    /// @return True if the object was found and removed, false otherwise.
    template<typename ObjectT>
    bool TryRemove(const Uuid& uuid);

    /// @brief Gets the number of objects of a specific type.
    /// @tparam ObjectT The type of object to count.
    /// @return Number of objects of the specified type.
    template<typename ObjectT>
    size_t GetCount() const;

    /// @brief Gets the total number of objects across all types.
    /// @return Total number of managed objects.
    size_t GetTotalCount() const;

    /// @brief Checks if any objects of the specified type exist.
    /// @tparam ObjectT The type to check for.
    /// @return True if any objects of this type exist, false otherwise.
    template<typename ObjectT>
    bool HasAny() const;

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    TypedObjectStorageMap _objects; ///< @brief Nested map: type -> (UUID -> object)

    // Private Methods

    template<typename ObjectT>
    void Register(std::unique_ptr<ObjectT> obj);

    template<typename ObjectT, typename StorageT>
    void RegisterAs(std::unique_ptr<ObjectT> obj);
};

} // namespace velecs::ecs

#include "velecs/ecs/World.inl"
