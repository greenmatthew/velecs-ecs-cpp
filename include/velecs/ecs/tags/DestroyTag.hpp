/// @file    DestroyTag.hpp
/// @author  Matthew Green
/// @date    2025-05-16 12:13:47
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include "velecs/ecs/Tag.hpp"

namespace velecs::ecs {

/// @class DestroyTag
/// @brief Marker tag that indicates an entity is scheduled for destruction.
///
/// The DestroyTag tag is used to mark entities that should be destroyed
/// during the next processing of the destruction queue. This allows for deferred
/// destruction of entities and provides a way to identify entities that are pending
/// deletion without immediately removing them from the registry.
class DestroyTag : public Tag {};

} // namespace velecs::ecs
