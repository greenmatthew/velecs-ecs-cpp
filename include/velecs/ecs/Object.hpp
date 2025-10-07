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
    friend class World;

public:
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

    template<typename ObjectT, typename... Args>
    static ObjectT* Create(World* const world, Args&&... args);

    template<typename StorageT, typename ObjectT, typename... Args>
    static ObjectT* CreateAs(World* const world, Args&&... args);

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

#include "velecs/ecs/Object.inl"

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
