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
    :
        pos(Vec3::ZERO),
        scale(Vec3::ONE),
        rot(Quat::IDENTITY),
        cachedModelMat(Mat4::IDENTITY),
        cachedWorldMat(Mat4::IDENTITY) {}

// Public Methods

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

void Transform::SetParent(const Entity newParent)
{
    if (_parent == newParent) return;  // No change needed
    
    const Entity owner = GetOwner();
    
    // Remove from current parent's children list
    if (_parent.IsValid()) {
        auto& oldParentTransform = _parent.GetTransform();
        oldParentTransform._children.erase(
            std::remove(oldParentTransform._children.begin(), 
                       oldParentTransform._children.end(), owner),
            oldParentTransform._children.end()
        );
    }
    
    // Set new parent
    _parent = newParent;
    
    // Add to new parent's children list
    if (_parent.IsValid()) {
        auto& newParentTransform = _parent.GetTransform();
        // Check if we're already in the list (shouldn't happen, but safety first)
        if (std::find(newParentTransform._children.begin(), 
                     newParentTransform._children.end(), owner) 
            == newParentTransform._children.end()) {
            newParentTransform._children.push_back(owner);
        }
    }
    
    SetWorldDirty();
}

bool Transform::HasChild(const Entity child) const
{
    return std::find(_children.begin(), _children.end(), child) != _children.end();
}

Entity Transform::TryGetChild(const size_t index) const
{
    if (index >= _children.size()) return Entity::INVALID;
    return _children[index];
}

bool Transform::TryAddChild(const Entity child)
{
    if (!child.IsValid() || child == GetOwner()) return false;
    
    // SetParent handles all the bidirectional relationship logic
    child.GetTransform().SetParent(GetOwner());
    return true;
}

bool Transform::TryRemoveChild(const Entity child)
{
    auto it = std::find(_children.begin(), _children.end(), child);
    if (it == _children.end()) return false;
    
    // SetParent(INVALID) will handle removing from our children list
    if (child.IsValid())
    {
        child.GetTransform().SetParent(Entity::INVALID);
    }
    
    return true;
}

bool Transform::TryRemoveChild(const size_t index)
{
    if (index >= _children.size()) return false;
    
    Entity child = _children[index];
    
    if (child.IsValid())
    {
        child.GetTransform().SetParent(Entity::INVALID);
    }

    return true;
}

size_t Transform::GetSiblingIndex() const
{
    if (!_parent.IsValid()) return 0;
    
    const auto& siblings = _parent.GetTransform()._children;
    auto it = std::find(siblings.begin(), siblings.end(), GetOwner());
    
    return (it != siblings.end()) ? std::distance(siblings.begin(), it) : 0;
}

void Transform::SetSiblingIndex(const size_t index)
{
    if (!_parent.IsValid()) return;
    
    auto& parentTransform = _parent.GetTransform();
    auto& siblings = parentTransform._children;
    
    // Find our current position
    auto it = std::find(siblings.begin(), siblings.end(), GetOwner());
    if (it == siblings.end()) return;  // We're not actually a child?
    
    // Remove from current position
    Entity self = *it;
    siblings.erase(it);
    
    // Insert at new position (clamped to valid range)
    size_t clampedIndex = std::min(index, siblings.size());
    siblings.insert(siblings.begin() + clampedIndex, self);
}

void Transform::SetAsFirstSibling()
{
    SetSiblingIndex(0);
}

void Transform::SetAsLastSibling()
{
    if (!_parent.IsValid()) return;
    SetSiblingIndex(_parent.GetTransform().GetChildCount());
}

bool Transform::IsChildOf(const Entity parent) const
{
    return _parent == parent;
}

bool Transform::IsDescendantOf(const Entity ancestor) const
{
    if (!ancestor) return false;
    
    Entity current = _parent;
    while (current)
    {
        if (current == ancestor) return true;
        current = current.GetTransform()._parent;
    }
    return false;
}

bool Transform::IsAncestorOf(const Entity descendant) const
{
    return descendant.IsValid() && descendant.GetTransform().IsDescendantOf(GetOwner());
}

Entity Transform::GetRoot() const
{
    Entity current = GetOwner();
    while (current.GetTransform()._parent.IsValid())
    {
        current = current.GetTransform()._parent;
    }
    return current;
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
    if (!_parent.IsValid()) return GetModelMatrix();
    return _parent.GetTransform().GetWorldMatrix() * GetModelMatrix();
}

void Transform::SetWorldDirty()
{
    isWorldDirty = true;
    for (const Entity& child : _children)
    {
        if (child.IsValid()) child.GetTransform().SetWorldDirty();
    }
}

void Transform::SetDirty()
{
    isModelDirty = true;
    SetWorldDirty();
}

} // namespace velecs::ecs
