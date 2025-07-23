/// @file    Relationship.cpp
/// @author  Matthew Green
/// @date    2025-05-16 11:19:41
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#include "velecs/ecs/components/Relationship.hpp"

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

// Public Methods

Entity Relationship::iterator::operator*()
{
    return current;
}

Entity Relationship::iterator::operator->()
{
    return current;
}

Relationship::iterator& Relationship::iterator::operator++()
{
    if (index < size - 1)
    {
        // Safety check: ensure current entity is valid before accessing its relationship
        if (current)
        {
            auto& relationship = current.GetRelationship();
            if (traverseForward) 
            {
                current = relationship.GetNextSibling();
            }
            else 
            {
                current = relationship.GetPrevSibling();
            }
        }
        else
        {
            current = Entity::INVALID; // Ensure we stay invalid if we were already invalid
        }
        index++;
    }
    else
    {
        // We've processed all elements, mark as end
        current = Entity::INVALID;
        index = size;  // Set index to size to indicate we're at the end
    }
    return *this;
}

Relationship::iterator Relationship::iterator::operator++(int)
{
    iterator tmp = *this;
    ++(*this);
    return tmp;
}

bool Relationship::iterator::operator==(const iterator& other) const
{
    return current == other.current;
}

bool Relationship::iterator::operator!=(const iterator& other) const
{
    return !(*this == other);
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

} // namespace velecs::ecs
