/// @file    EntityPersistence.hpp
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

/// @struct EntityPersistence
/// @brief Brief description.
///
/// Rest of description.
struct EntityPersistence {
    friend class Entity;

public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor.
    EntityPersistence(std::unique_ptr<Entity> entity)
        : entityPtr(std::move(entity)) {}

    /// @brief Default deconstructor.
    ~EntityPersistence() = default;

    // Public Methods

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    // Private Methods

    std::unique_ptr<Entity> entityPtr;

    inline EntityRef CreateEntityRef()
    {
        Entity** rawEntityPtrPtr = reinterpret_cast<Entity**>(&entityPtr);
        return EntityRef(rawEntityPtrPtr);
    }

    inline std::unique_ptr<Entity> Transfer()
    {
        return std::move(entityPtr);
    }
};

} // namespace velecs::ecs
