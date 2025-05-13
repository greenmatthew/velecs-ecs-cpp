/// @file    SceneNodeRef.hpp
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

class SceneNode;

/// @class SceneNodeRef
/// @brief A lightweight reference wrapper for SceneNode objects.
///
/// SceneNodeRef provides a non-owning reference to an SceneNode managed by a unique_ptr.
/// It allows accessing the SceneNode through pointer semantics without ownership transfer.
/// This class is designed for performance with minimal overhead, intentionally
/// omitting safety checks in operators for efficiency - users must verify validity
/// before access using IsAlive().
template <typename T = SceneNode>
class SceneNodeRef {
    friend class SceneNode;
    friend class Entity;

public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor deleted - SceneNodeRef must reference something.
    SceneNodeRef() = delete;

    /// @brief Default destructor.
    ~SceneNodeRef() = default;

    // Public Methods

    /// @brief Arrow operator for accessing SceneNode methods.
    /// @return Pointer to the referenced SceneNode.
    /// @note No validity check is performed for performance reasons.
    inline T* operator->() const
    {
        return *sceneNodePtrPtr;
    }
    
    /// @brief Dereference operator to get a reference to the SceneNode.
    /// @return Reference to the SceneNode.
    /// @note No validity check is performed for performance reasons.
    inline T& operator*() const
    {
        return **sceneNodePtrPtr;
    }
    
    /// @brief Get the raw SceneNode pointer.
    /// @return Pointer to the referenced SceneNode.
    /// @note No validity check is performed for performance reasons.
    inline T* get() const
    {
        return *sceneNodePtrPtr;
    }

    /// @brief Cast to a reference of a different SceneNode type
    /// @return A SceneNodeRef of the requested type
    /// @note This will compile but may cause runtime errors if the cast is invalid
    template <typename U>
    inline SceneNodeRef<U> As() const {
        return SceneNodeRef<U>(reinterpret_cast<U**>(nodePtrPtr));
    }

    /// @brief Boolean conversion operator that checks if the reference is valid.
    /// @return true if the SceneNode reference is valid, false otherwise.
    inline explicit operator bool() const
    {
        return IsAlive();
    }

    /// @brief Checks if the referenced SceneNode is still alive.
    /// @return true if the SceneNode pointer is valid, false otherwise.
    inline bool IsAlive() const
    {
        return sceneNodePtrPtr != nullptr && *sceneNodePtrPtr != nullptr;
    }

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    /// @brief Double pointer to the SceneNode.
    /// Holds a pointer to the pointer held by the unique_ptr.
    T** const sceneNodePtrPtr;

    // Private Methods

    inline explicit SceneNodeRef(T** sceneNodePtrPtr)
        : sceneNodePtrPtr(sceneNodePtrPtr) {}
};

} // namespace velecs::ecs