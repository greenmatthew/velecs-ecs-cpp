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

    inline Entity GetParent() const
    {
        return parent;
    }

    void SetParent(const Entity& newParent);
    
    inline const std::vector<Entity>& GetChildren() const
    {
        return children;
    }

    inline size_t GetChildCount() const
    {
        return children.size();
    }

    /// @brief Safely try to get child entity at the specified index
    /// @param index Index of the child to retrieve
    /// @param outChild Pointer that will be set to the child if found
    /// @return True if the child was found, false otherwise
    inline bool TryGetChild(const size_t index, Entity& outChild) const
    {
        if (index < children.size())
        {
            outChild = children[index];
            return true;
        }
        return false;
    }

    void AddChild(const Entity& child);

    // void AddChild(const Entity& child, const unsigned int index);

    void RemoveChild(const Entity& child);
    
    // void RemoveChild(const Entity& child, const unsigned int index);

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

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    Entity parent;
    std::vector<Entity> children;
    std::unordered_set<Entity> childrenHandles;

    math::Vec3 pos;
    math::Vec3 scale;
    math::Quat rot;

    mutable bool isDirty = true;
    mutable math::Mat4 cachedModelMat;

    // Private Methods

    math::Mat4 CalculateModel() const;
};

} // namespace velecs::ecs
