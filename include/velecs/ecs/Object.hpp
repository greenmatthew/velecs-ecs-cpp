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

class ObjectManager;

/// @class Object
/// @brief Brief description.
///
/// Rest of description.
class Object {
public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Constructor access key to enforce controlled object creation.
    class ConstructorKey {
        friend class Object;
        ConstructorKey() = default;
    public:
        virtual ~ConstructorKey() = default;
    };

    /// @brief Constructor with manager and default name.
    /// @param manager Pointer to the managing ObjectManager.
    /// @param key Constructor access key for controlled instantiation.
    inline Object(ObjectManager* const manager, ConstructorKey)
        : _manager(manager) {}

    /// @brief Constructor with manager and custom name.
    /// @param manager Pointer to the managing ObjectManager.
    /// @param name The initial name for this object.
    /// @param key Constructor access key for controlled instantiation.
    inline Object(ObjectManager* const manager, const std::string& name, ConstructorKey)
        : _manager(manager), _name(name) {}

    /// @brief Creates an object of the specified type with perfect forwarding of constructor arguments.
    /// @tparam ObjectT The type of object to create. Must inherit from Object.
    /// @tparam Args The types of constructor arguments (automatically deduced).
    /// @param manager Pointer to the object manager.
    /// @param args Constructor arguments to forward to the ObjectT constructor.
    /// @return Pointer to the newly created object of type ObjectT.
    /// @details Uses perfect forwarding to support any constructor signature.
    ///          The manager parameter is always passed first, followed by the forwarded args,
    ///          and finally the ConstructorKey for access control.
    template<typename ObjectT, typename... Args>
    static ObjectT* Create(ObjectManager* const manager, Args&&... args)
    {
        static_assert(std::is_base_of_v<Object, ObjectT>, "ObjectT must inherit from Object");
        
        auto objStorage = std::make_unique<ObjectT>(manager, std::forward<Args>(args)..., ConstructorKey{});
        auto obj = objStorage.get();
        auto uuid = Uuid::GenerateRandom();
        obj->_uuid = uuid;
        manager->Register<ObjectT>(std::move(objStorage));
        return obj;
    }

    /// @brief Deleted default constructor.
    /// @details Objects must always be associated with a manager.
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
    /// @return True if objects have the same UUID and manager, false otherwise.
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
    /// @return True if the object has a valid manager and UUID, false otherwise.
    bool IsValid() const;

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

private:
    // Private Fields

    ObjectManager* _manager{nullptr}; ///< @brief Pointer to the managing ObjectManager
    Uuid _uuid{Uuid::INVALID};        ///< @brief Unique identifier for this object
    std::string _name{"Object"};      ///< @brief Human-readable name for this object

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
