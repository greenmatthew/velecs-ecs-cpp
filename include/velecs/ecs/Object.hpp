/// @file    Object.hpp
/// @author  Matthew Green
/// @date    2025-08-09 12:42:26
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include <velecs/common/Uuid.hpp>
using velecs::common::Uuid;

#include <string>
#include <iostream>

namespace velecs::ecs {

class World;

/// @class Object
/// @brief Brief description.
///
/// Rest of description.
class Object {
public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Constructor with world and default name.
    /// @param world Pointer to the managing World.
    inline Object(World* const world)
        : _world(world) {}

    /// @brief Constructor with world and custom name.
    /// @param world Pointer to the managing World.
    /// @param name The initial name for this object.
    inline Object(World* const world, const std::string& name)
        : _world(world), _name(name) {}

    /// @brief Creates an object of the specified type with perfect forwarding of constructor arguments.
    /// @tparam ObjectT The type of object to create. Must inherit from Object.
    /// @tparam Args The types of constructor arguments (automatically deduced).
    /// @param world Pointer to the object world.
    /// @param args Constructor arguments to forward to the ObjectT constructor.
    /// @return Pointer to the newly created object of type ObjectT.
    /// @details Uses perfect forwarding to support any constructor signature.
    ///          The world parameter is always passed first, followed by the forwarded args,
    ///          and finally the ConstructorKey for access control.
    template<typename ObjectT, typename... Args>
    static ObjectT* Create(Args&&... args)
    {
        static_assert(std::is_base_of_v<Object, ObjectT>, "ObjectT must inherit from Object");
        
        auto objStorage = std::make_unique<ObjectT>(std::forward<Args>(args)...);
        auto obj = objStorage.get();
        auto uuid = Uuid::GenerateRandom();
        obj->_uuid = uuid;
        obj->GetWorld()->Internal_Register<ObjectT>(std::move(objStorage));
        return obj;
    }

    /// @brief Creates an object but stores it under a different type for polymorphic lookup.
    /// @tparam StorageT The type to store as (usually base class).
    /// @tparam ObjectT The concrete type to create.
    /// @tparam Args Constructor argument types.
    /// @param world Pointer to the object world.
    /// @param args Constructor arguments to forward.
    /// @return Pointer to the newly created object of type ObjectT.
    template<typename StorageT, typename ObjectT, typename... Args>
    static ObjectT* CreateAs(Args&&... args)
    {
        static_assert(std::is_base_of_v<Object, ObjectT>, "ObjectT must inherit from Object");
        static_assert(std::is_base_of_v<StorageT, ObjectT>, "ObjectT must inherit from StorageT");
        
        auto objStorage = std::unique_ptr<StorageT>(new ObjectT(std::forward<Args>(args)...));
        auto* obj = objStorage.get();
        obj->_uuid = Uuid::GenerateRandom();
        obj->GetWorld()->Internal_Register<StorageT>(std::move(objStorage));

        return static_cast<ObjectT*>(obj);
    }

    /// @brief Deleted default constructor.
    /// @details Objects must always be associated with a world.
    Object() = delete;

    /// @brief Virtual destructor for proper cleanup of derived classes.
    virtual ~Object() = default;

    // Public Methods

    /// @brief Assignment operator.
    /// @param other The object to copy from.
    /// @return Reference to this object after assignment.
    Object& operator=(const Object& other) noexcept;

    /// @brief Boolean conversion operator.
    /// @return True if the object is valid, false otherwise.
    /// @details Allows objects to be used in conditional expressions.
    inline explicit operator bool() const { return IsValid(); }

    /// @brief Equality comparison operator.
    /// @param other The object to compare with.
    /// @return True if objects have the same UUID and world, false otherwise.
    bool operator==(const Object& other) const;

    /// @brief Inequality comparison operator.
    /// @param other The object to compare with.
    /// @return True if objects have different UUIDs or managers, false otherwise.
    bool operator!=(const Object& other) const;

    /// @brief Gets the hash code for this object.
    /// @return Hash value based on the object's UUID.
    /// @details Used for storing objects in hash-based containers.
    size_t GetHashCode() const;

    /// @brief Gets a string representation of this object.
    /// @return String containing object name and UUID.
    std::string ToString() const;

    /// @brief Checks if this object is valid.
    /// @return True if the object has a valid world and UUID, false otherwise.
    inline bool IsValid() const { return _world != nullptr && _uuid != Uuid::INVALID; }

    /// @brief Gets the object's UUID.
    /// @return The unique identifier for this object.
    inline const Uuid& GetUuid() const { return _uuid; }

    /// @brief Gets the object's name.
    /// @return The current name of this object.
    inline const std::string& GetName() const { return _name; }

    /// @brief Sets the object's name.
    /// @param name The new name to assign to this object.
    inline void SetName(const std::string& name) { _name = name; }

    /// @brief Stream output operator for easy debugging and logging.
    /// @param os Output stream to write to.
    /// @param obj Object to output.
    /// @return Reference to the output stream.
    friend std::ostream& operator<<(std::ostream& os, const Object& obj);

protected:
    // Protected Fields

    // Protected Methods
    
    inline World* const GetWorld() const { return _world; }

private:
    // Private Fields

    World* _world{nullptr};      ///< @brief Pointer to the managing World
    Uuid _uuid{Uuid::INVALID};   ///< @brief Unique identifier for this object
    std::string _name{"Object"}; ///< @brief Human-readable name for this object

    // Private Methods
};

} // namespace velecs::ecs



/// @brief Standard library specialization for hashing Object instances.
namespace std {

/// @brief Hash specialization for Object instances.
/// @details Enables Object instances to be used as keys in std::unordered_map and std::unordered_set.
template <>
struct hash<velecs::ecs::Object> {
    /// @brief Computes hash value for an Object.
    /// @param obj The object to compute the hash for.
    /// @return Hash value for the object.
    size_t operator()(const velecs::ecs::Object& obj) const
    {
        return obj.GetHashCode();
    }
};

} // namespace std
