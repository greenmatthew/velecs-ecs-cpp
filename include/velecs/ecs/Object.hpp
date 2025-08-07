/// @file    Object.hpp
/// @author  Matthew Green
/// @date    2025-08-07 15:53:31
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include <entt/entt.hpp>

#include <string>
#include <optional>
#include <functional>

namespace velecs::ecs {

/// @class Object
/// @brief Brief description.
///
/// Rest of description.
class Object {
public:
    // Enums

    // Public Fields

    static const Object INVALID;

    // Constructors and Destructors

    /// @brief Default constructor.
    Object() = default;
    
    /// @brief Copy constructor.
    /// @param other The object to copy from.
    Object(const Object& other) = default;

    /// @brief Move constructor.
    /// @param other The object to move from.
    Object(Object&& other) noexcept = default;

    /// @brief Copy assignment operator.
    /// @param other The object to copy from.
    /// @return Reference to this object.
    Object& operator=(const Object& other) = default;

    /// @brief Move assignment operator.
    /// @param other The object to move from.
    /// @return Reference to this object.
    Object& operator=(Object&& other) noexcept = default;
    
    /// @brief Default destructor.
    virtual ~Object() = default;

    // Public Methods

    /// @brief Equality comparison operator.
    /// @param other The object to compare with.
    /// @return True if both objects reference the same registry and handle.
    bool operator==(const Object& other) const;

    /// @brief Inequality comparison operator.
    /// @param other The object to compare with.
    /// @return True if objects reference different registries or handles.
    bool operator!=(const Object& other) const;

    /// @brief Boolean conversion operator.
    /// @return True if the object is valid, false otherwise.
    /// @details Allows objects to be used in conditional expressions.
    inline explicit operator bool() const { return IsValid(); }

    /// @brief Gets the hash code for this object.
    /// @return Hash value based on the registry pointer and entity handle.
    /// @details Used for storing objects in hash-based containers.
    size_t GetHashCode() const;

    /// @brief Checks if this object is valid.
    /// @return True if the object has a valid registry and handle, false otherwise.
    /// @details An object is considered valid if it has a registry reference and
    ///          the handle is valid within that registry.
    bool IsValid() const;

    
    /// @brief Internal tag used to mark objects for deferred destruction.
    /// @details This tag is applied to objects when RequestDestroy() is called.
    ///          The destruction system processes all objects with this tag during
    ///          cleanup phases, allowing for safe deferred destruction without
    ///          immediate registry modifications during processing loops.
    struct RequestDestructionTag {};
    
    /// @brief Marks this object for deferred destruction.
    /// @details Adds a RequestDestructionTag to this object, scheduling it for
    ///          destruction during the next cleanup phase. This allows safe
    ///          destruction requests during system processing without immediately
    ///          invalidating iterators or causing race conditions.
    ///          Only valid objects can be marked for destruction.
    void RequestDestroy();

    /// @brief Gets the name of this object.
    /// @return The current name of the object.
    const std::string& GetName() const;

    /// @brief Sets the name of this object.
    /// @param name The new name to assign to this object.
    /// @details Only valid objects can have their names changed.
    ///          Invalid objects will ignore name changes.
    void SetName(const std::string& name);

protected:
    // Protected Fields

    // Protected Methods

    /// @brief Protected constructor for creating valid objects.
    /// @param registry Pointer to the EnTT registry.
    /// @param handle The entity handle within the registry.
    /// @param name The initial name for this object.
    /// @details This constructor is protected to ensure only derived classes
    ///          and friend classes can create valid objects.
    Object(entt::registry* registry, entt::entity handle, const std::string& name);

private:
    // Private Fields

    entt::registry* _registry{nullptr};     ///< @brief Pointer to the EnTT registry
    entt::entity _handle{entt::null};       ///< @brief Entity handle within the registry
    std::string _name{"Invalid Object"};    ///< @brief Human-readable name for this object

    // Private Methods

    /// @brief Immediately destroys this object and invalidates it.
    /// @details Removes the object from the registry and resets this instance
    ///          to an invalid state. Unlike RequestDestroy(), this performs
    ///          immediate destruction and should be used carefully to avoid
    ///          iterator invalidation during processing loops.
    ///          Safe to call on invalid objects (no-op).
    void Destroy();

    /// @brief Resets this object to an invalid state.
    /// @details Clears the registry pointer, sets handle to null, and resets
    ///          the name to "Invalid Object". This is used internally during
    ///          destruction and error handling to ensure consistent invalid state.
    void Reset();
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
