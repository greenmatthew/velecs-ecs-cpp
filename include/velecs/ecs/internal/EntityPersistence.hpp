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
public:
    // Enums

    // Public Fields

    std::unique_ptr<Entity> entity;

    // Constructors and Destructors

    /// @brief Default constructor.
    EntityPersistence(std::unique_ptr<Entity> entity)
        : entity(std::move(entity)) {}

    /// @brief Default deconstructor.
    ~EntityPersistence() = default;

    // Public Methods

    inline std::unique_ptr<Entity> Transfer()
    {
        return std::move(entity);
    }

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    // Private Methods
};

} // namespace velecs::ecs
