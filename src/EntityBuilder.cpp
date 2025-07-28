/// @file    EntityBuilder.cpp
/// @author  Matthew Green
/// @date    2025-05-16 11:44:30
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#include "velecs/ecs/EntityBuilder.hpp"

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

EntityBuilder::EntityBuilder(Scene* const scene, const entt::entity handle)
    : entity(Entity(scene, handle)),
        name(entity.AddComponent<Name>("Entity")),
        transform(entity.AddComponent<Transform>()) {}

// Public Methods

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

} // namespace velecs::ecs
