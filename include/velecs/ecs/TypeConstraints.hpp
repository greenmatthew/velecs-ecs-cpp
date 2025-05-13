/// @file    TypeConstraints.hpp
/// @author  Matthew Green
/// @date    2025-05-12 17:32:24
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include <type_traits>

namespace velecs::ecs {

class Entity;
class Component;
class SceneNode;

template <typename T>
using IsComponent = std::enable_if_t<std::is_base_of_v<Component, T>>;

template <typename T>
using IsSceneNode = std::enable_if_t<std::is_base_of_v<SceneNode, T>>;

} // namespace velecs::ecs
