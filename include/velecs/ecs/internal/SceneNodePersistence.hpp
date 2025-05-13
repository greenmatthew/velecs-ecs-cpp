/// @file    SceneNodePersistence.hpp
/// @author  Matthew Green
/// @date    2025-05-13 10:43:18
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

/// @struct SceneNodePersistence
/// @brief Brief description.
///
/// Rest of description.
struct SceneNodePersistence {
    friend class Entity;

public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor.
    SceneNodePersistence(std::unique_ptr<Entity> entity)
        : entityPtr(std::move(entity)) {}

    /// @brief Default deconstructor.
    ~SceneNodePersistence() = default;

    // Public Methods

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    // Private Methods

    std::unique_ptr<Entity> entityPtr;

    inline SceneNode** GetUnsafePtrPtr()
    {
        return reinterpret_cast<SceneNode**>(&entityPtr);
    }

    inline std::unique_ptr<Entity> Transfer()
    {
        return std::move(entityPtr);
    }
};

} // namespace velecs::ecs
