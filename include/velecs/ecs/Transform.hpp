/// @file    Transform.hpp
/// @author  Matthew Green
/// @date    2025-05-12 17:48:04
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include "velecs/ecs/Component.hpp"

#include "velecs/math/Vec3.hpp"

namespace velecs::ecs {

/// @class Transform
/// @brief Brief description.
///
/// Rest of description.
class Transform : public Component {
public:
    // Enums

    // Public Fields

    math::Vec3 pos;
    math::Vec3 scale;
    math::Vec3 rot;

    // Constructors and Destructors

    /// @brief Default constructor.
    Transform();

    /// @brief Default deconstructor.
    ~Transform() = default;

    // Public Methods

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    // Private Methods
};

} // namespace velecs::ecs
