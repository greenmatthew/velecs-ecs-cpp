#include "velecs/ecs/components/Transform.hpp"

using namespace velecs::math;

#include <sstream>

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

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

Vec3 Transform::GetEulerAnglesRad() const
{
    return rot.ToEulerAnglesRad();
}

Vec3 Transform::GetEulerAnglesDeg() const
{
    return rot.ToEulerAnglesDeg();
}

void Transform::SetEulerAnglesRad(const Vec3& newAnglesRad)
{
    rot = Quat::FromEulerAnglesRad(newAnglesRad);
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

bool Transform::TrySetParent(Entity* const newParent)
{
    // Only use a valid entity as a parent.
    if (!newParent || !newParent->IsValid()) return false;

    Entity* const owner = GetOwner();

    // The new parent must be apart of the same scene.
    if (owner->GetScene() != newParent->GetScene()) return false;

    assert(GetScene() == newParent->GetScene() && "Must use the same scene");

    // If its already the parent then no change needed.
    if (HasParent(newParent)) return true;
    
    // Remove from current parent's children list
    if (_parent != nullptr && _parent->IsValid())
    {
        auto& oldParentTransform = _parent->GetTransform();
        oldParentTransform._children.erase(
            std::remove(oldParentTransform._children.begin(), 
                       oldParentTransform._children.end(), owner),
            oldParentTransform._children.end()
        );
    }
    
    // Set new parent
    _parent = newParent;
    
    // Add to new parent's children list
    if (_parent->IsValid())
    {
        auto& newParentTransform = _parent->GetTransform();
        // Check if we're already in the list (shouldn't happen, but safety first)
        if (std::find(newParentTransform._children.begin(), 
                     newParentTransform._children.end(), owner) 
            == newParentTransform._children.end()) {
            newParentTransform._children.push_back(owner);
        }
    }
    
    SetWorldDirty();
    return true;
}

bool Transform::HasChild(const Entity* const child) const
{
    return std::find(_children.begin(), _children.end(), child) != _children.end();
}

Entity* Transform::TryGetChild(const size_t index) const
{
    if (index >= _children.size()) return nullptr;
    return _children[index];
}

bool Transform::TryAddChild(Entity* const child)
{
    assert(GetScene() == child->GetScene() && "Must use the same scene");

    if (!child->IsValid() || child == GetOwner()) return false;
    
    // SetParent handles all the bidirectional relationship logic
    child->GetTransform().TrySetParent(GetOwner());
    return true;
}

bool Transform::TryRemoveChild(Entity* const child)
{
    assert(GetScene() == child->GetScene() && "Must use the same scene");

    auto it = std::find(_children.begin(), _children.end(), child);
    if (it == _children.end()) return false;
    
    // SetParent(INVALID) will handle removing from our children list
    if (child->IsValid())
    {
        child->GetTransform().TrySetParent(nullptr);
    }
    
    return true;
}

bool Transform::TryRemoveChild(const size_t index)
{
    if (index >= _children.size()) return false;
    
    Entity* const child = _children[index];

    assert(GetScene() == child->GetScene() && "Must use the same scene");
    
    if (child->IsValid())
    {
        child->GetTransform().TrySetParent(nullptr);
    }

    return true;
}

size_t Transform::GetSiblingIndex() const
{
    if (!_parent->IsValid()) return 0;
    
    const auto& siblings = _parent->GetTransform()._children;
    auto it = std::find(siblings.begin(), siblings.end(), GetOwner());
    
    return (it != siblings.end()) ? std::distance(siblings.begin(), it) : 0;
}

bool Transform::TrySetSiblingIndex(const size_t index)
{
    if (!_parent->IsValid()) return false;
    
    auto& parentTransform = _parent->GetTransform();
    auto& siblings = parentTransform._children;
    
    // Find our current position
    auto it = std::find(siblings.begin(), siblings.end(), GetOwner());

    // CRITICAL INVARIANT VIOLATION: We claim to have a parent but aren't in their children list
    if (it == siblings.end())
    {
        std::ostringstream oss;
        oss << "Transform hierarchy corruption detected: Entity claims parent but isn't in parent's children list. "
            << "This indicates a serious bug in the ECS hierarchy system.";
        throw std::runtime_error(oss.str());
    }  
    
    // Remove from current position
    Entity* const self = *it;
    siblings.erase(it);
    
    // Insert at new position (clamped to valid range)
    size_t clampedIndex = std::min(index, siblings.size());
    siblings.insert(siblings.begin() + clampedIndex, self);
    return true;
}

bool Transform::TrySetAsFirstSibling()
{
    return TrySetSiblingIndex(0);
}

bool Transform::TrySetAsLastSibling()
{
    if (!_parent->IsValid()) return false;
    return TrySetSiblingIndex(_parent->GetTransform().GetChildCount());
}

bool Transform::IsChildOf(const Entity* const parent) const
{
    return _parent == parent;
}

bool Transform::IsDescendantOf(const Entity* const ancestor) const
{
    assert(GetScene() == ancestor->GetScene() && "Must use the same scene");

    if (!ancestor) return false;
    
    Entity* current = _parent;
    while (current)
    {
        if (current == ancestor) return true;
        current = current->GetTransform()._parent;
    }
    return false;
}

bool Transform::IsAncestorOf(const Entity* const descendant) const
{
    assert(GetScene() == descendant->GetScene() && "Must use the same scene");

    return descendant->IsValid() && descendant->GetTransform().IsDescendantOf(GetOwner());
}

Entity* Transform::GetRoot() const
{
    Entity* current = GetOwner();
    while (current->GetTransform()._parent->IsValid())
    {
        current = current->GetTransform()._parent;
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
    if (!_parent) return GetModelMatrix();
    return _parent->GetTransform().GetWorldMatrix() * GetModelMatrix();
}

void Transform::SetWorldDirty()
{
    isWorldDirty = true;
    for (const Entity* child : _children)
    {
        if (child->IsValid()) child->GetTransform().SetWorldDirty(); 
    }
}

void Transform::SetDirty()
{
    isModelDirty = true;
    SetWorldDirty();
}

} // namespace velecs::ecs
