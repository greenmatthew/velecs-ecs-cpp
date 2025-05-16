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
#include "velecs/math/Quat.hpp"
#include "velecs/math/Mat4.hpp"

#include "velecs/ecs/Entity.hpp"

#include <vector>

namespace velecs::ecs {

/// @class Transform
/// @brief Brief description.
///
/// Rest of description.
class Transform : public Component {
public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor.
    Transform();

    /// @brief Default deconstructor.
    ~Transform() = default;

    // Public Methods

    math::Vec3 GetPos() const;
    void SetPos(const math::Vec3& newPos);

    math::Vec3 GetScale() const;
    void SetScale(const math::Vec3& newScale);

    math::Vec3 GetEulerAngles() const;
    math::Vec3 GetEulerAnglesDeg() const;
    void SetEulerAngles(const math::Vec3& newAngles);
    void SetEulerAnglesDeg(const math::Vec3& newAnglesDeg);

    math::Quat GetRot() const;
    void SetRot(const math::Quat& newRot);

    math::Mat4 GetModelMatrix() const;

    math::Mat4 GetWorldMatrix() const;

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    math::Vec3 pos;
    math::Vec3 scale;
    math::Quat rot;

    mutable bool isModelDirty = true;
    mutable math::Mat4 cachedModelMat;

    mutable bool isWorldDirty = true;
    mutable math::Mat4 cachedWorldMat;

    // Private Methods

    math::Mat4 CalculateModel() const;
    math::Mat4 CalculateWorld() const;

    void SetWorldDirty();
    void SetDirty();
};

} // namespace velecs::ecs
