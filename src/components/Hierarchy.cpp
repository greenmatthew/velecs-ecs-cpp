/// @file    Hierarchy.cpp
/// @author  Matthew Green
/// @date    2025-05-14 11:09:05
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#include "velecs/ecs/components/Hierarchy.hpp"

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

// Public Methods

void Hierarchy::SetParent(const Entity& newParent)
{
    Hierarchy* parentHierarchy = nullptr;
    if (parent.TryGetComponent<Hierarchy>(parentHierarchy)) parentHierarchy->RemoveChild(GetOwner());
    parent = newParent;
}

void Hierarchy::AddChild(const Entity& child)
{
    // Only add if not already a child
    if (childrenHandles.find(child) == childrenHandles.end())
    {
        children.emplace_back(child);
        childrenHandles.insert(child);
    }
}

void Hierarchy::RemoveChild(const Entity& child)
{
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end())
    {
        childrenHandles.erase(child);
        children.erase(it);
    }
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

} // namespace velecs::ecs
