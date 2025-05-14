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
    : parent(Entity::INVALID),
        pos(Vec3::ZERO),
        scale(Vec3::ONE),
        rot(Quat::IDENTITY),
        cachedModelMat(Mat4::IDENTITY),
        cachedWorldMat(Mat4::IDENTITY) {}

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
    SetDirty();
}

Vec3 Transform::GetScale() const
{
    return scale;
}

void Transform::SetScale(const Vec3& newScale)
{
    scale = newScale;
    SetDirty();
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
    SetDirty();
}

void Transform::SetEulerAnglesDeg(const Vec3& newAnglesDeg)
{
    rot = Quat::FromEulerAnglesDeg(newAnglesDeg);
    SetDirty();
}

Quat Transform::GetRot() const
{
    return rot;
}

void Transform::SetRot(const Quat& newRot)
{
    rot = newRot;
    SetDirty();
}

Mat4 Transform::GetModelMatrix() const
{
    if (isModelDirty)
    {
        cachedModelMat = CalculateModel();
        isModelDirty = false;
    }
    return cachedModelMat;
}

Mat4 Transform::GetWorldMatrix() const
{
    if (isWorldDirty)
    {
        cachedWorldMat = CalculateWorld();
        isWorldDirty = false;
    }
    return cachedWorldMat;
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

Mat4 Transform::CalculateWorld() const
{
    // If there is no parent then the model is the world matrix.
    if (!parent) return GetModelMatrix();
    // If there is get the parent's world matrix and apply it to the model matrix.
    else return parent.GetTransform().GetWorldMatrix() * GetModelMatrix();
}

void Transform::SetWorldDirty()
{
    isWorldDirty = true;
    for (auto& child : GetChildren())
    {
        child.GetTransform().SetWorldDirty();
    }
}

void Transform::SetDirty()
{
    isModelDirty = true;
    SetWorldDirty();
}

} // namespace velecs::ecs
