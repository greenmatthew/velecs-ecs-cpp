/// @file    Hierarchy.hpp
/// @author  Matthew Green
/// @date    2025-05-14 11:13:08
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include "velecs/ecs/Component.hpp"
#include "velecs/ecs/Entity.hpp"

#include <vector>

namespace velecs::ecs {

class Entity;

/// @class Hierarchy
/// @brief Brief description.
///
/// Rest of description.
class Hierarchy : public Component {
public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor.
    Hierarchy()
        : parent(Entity::INVALID) {}

    /// @brief Default deconstructor.
    ~Hierarchy() = default;

    // Public Methods

    inline Entity GetParent() const
    {
        return parent;
    }

    void SetParent(const Entity& newParent);
    
    inline const std::vector<Entity>& const GetChildren() const
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

    void AddChild(const Entity& child, const unsigned int index);

    void RemoveChild(const Entity& child);
    
    void RemoveChild(const Entity& child, const unsigned int index);

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    Entity parent;
    std::vector<Entity> children;
    std::unordered_set<Entity> childrenHandles;

    // Private Methods
};

} // namespace velecs::ecs
