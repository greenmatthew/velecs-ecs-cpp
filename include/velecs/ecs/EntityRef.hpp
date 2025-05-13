/// @file    EntityRef.hpp
/// @author  Matthew Green
/// @date    2025-05-13 11:53:31
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include <memory>

namespace velecs::ecs {

class Entity;

/// @class EntityRef
/// @brief A lightweight reference wrapper for Entity objects.
///
/// EntityRef provides a non-owning reference to an Entity managed by a unique_ptr.
/// It allows accessing the Entity through pointer semantics without ownership transfer.
/// This class is designed for performance with minimal overhead, intentionally
/// omitting safety checks in operators for efficiency - users must verify validity
/// before access using IsAlive().
class EntityRef {
public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor deleted - EntityRef must reference something.
    EntityRef() = delete;

    /// @brief Default destructor.
    ~EntityRef() = default;

    // Public Methods

    /// @brief Arrow operator for accessing Entity methods.
    /// @return Pointer to the referenced Entity.
    /// @note No validity check is performed for performance reasons.
    inline Entity* operator->() const {
        return *entityPtrPtr;
    }
    
    /// @brief Dereference operator to get a reference to the Entity.
    /// @return Reference to the Entity.
    /// @note No validity check is performed for performance reasons.
    inline Entity& operator*() const {
        return **entityPtrPtr;
    }
    
    /// @brief Get the raw Entity pointer.
    /// @return Pointer to the referenced Entity.
    /// @note No validity check is performed for performance reasons.
    inline Entity* get() const {
        return *entityPtrPtr;
    }

    /// @brief Boolean conversion operator that checks if the reference is valid.
    /// @return true if the Entity reference is valid, false otherwise.
    inline explicit operator bool() const {
        return IsAlive();
    }

    /// @brief Checks if the referenced Entity is still alive.
    /// @return true if the Entity pointer is valid, false otherwise.
    inline bool IsAlive() const
    {
        return entityPtrPtr != nullptr && *entityPtrPtr != nullptr;
    }

protected:
    // Protected Fields

    // Protected Methods

private:

    friend class Entity;

    // Private Fields

    /// @brief Double pointer to the Entity.
    /// Holds a pointer to the pointer held by the unique_ptr.
    Entity** const entityPtrPtr;

    // Private Methods

    inline explicit EntityRef(Entity** entityPtrPtr)
        : entityPtrPtr(entityPtrPtr) {}
};

} // namespace velecs::ecs