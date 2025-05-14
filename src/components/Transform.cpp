/// @file    Transform.cpp
/// @author  Matthew Green
/// @date    2025-05-12 18:04:46
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#include "velecs/ecs/components/Transform.hpp"

using namespace velecs::math;

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

Transform::Transform()
    : parent(Entity::INVALID), pos(Vec3::ZERO), scale(Vec3::ONE), rot(Quat::IDENTITY), cachedModelMat(Mat4::IDENTITY) {}


// Public Methods

void Transform::SetParent(const Entity& newParent)
{
    // If the new parent is the same then ignore.
    if (parent == newParent) return;

    // If there is already a parent then remove it as a child
    if (parent) parent.GetTransform().RemoveChild(GetOwner());

    // Assign new parent.
    parent = newParent;
}

void Transform::AddChild(const Entity& child)
{
    // Only add if not already a child
    if (childrenHandles.find(child) == childrenHandles.end())
    {
        children.emplace_back(child);
        childrenHandles.insert(child);
    }
}

void Transform::RemoveChild(const Entity& child)
{
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end())
    {
        childrenHandles.erase(child);
        children.erase(it);
    }
}

Vec3 Transform::GetPos() const
{
    return pos;
}

void Transform::SetPos(const Vec3& newPos)
{
    pos = newPos;
    isDirty = true;
}

Vec3 Transform::GetScale() const
{
    return scale;
}

void Transform::SetScale(const Vec3& newScale)
{
    scale = newScale;
    isDirty = true;
}

Vec3 Transform::GetEulerAngles() const
{
    return rot.ToEulerAngles();
}

Vec3 Transform::GetEulerAnglesDeg() const
{
    return rot.ToEulerAnglesDeg();
}

void Transform::SetEulerAngles(const Vec3& newAngles)
{
    rot = Quat::FromEulerAngles(newAngles);
    isDirty = true;
}

void Transform::SetEulerAnglesDeg(const Vec3& newAnglesDeg)
{
    rot = Quat::FromEulerAnglesDeg(newAnglesDeg);
    isDirty = true;
}

Quat Transform::GetRot() const
{
    return rot;
}

void Transform::SetRot(const Quat& newRot)
{
    rot = newRot;
    isDirty = true;
}

Mat4 Transform::GetModelMatrix() const
{
    if (isDirty)
    {
        cachedModelMat = CalculateModel();
        isDirty = false;
    }
    return cachedModelMat;
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

Mat4 Transform::CalculateModel() const
{
    Mat4 T = Mat4::FromPosition(pos);
    Mat4 R = rot.ToMatrix();
    Mat4 S = Mat4::FromScale(scale);

    return T * R * S;
}

} // namespace velecs::ecs
