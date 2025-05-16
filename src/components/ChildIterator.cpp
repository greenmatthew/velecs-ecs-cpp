/// @file    ChildIterator.cpp
/// @author  Matthew Green
/// @date    2025-05-15 15:27:29
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#include "velecs/ecs/components/ChildIterator.hpp"

#include "velecs/ecs/components/Relationship.hpp"

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

// Public Methods

Entity ChildIterator::operator*()
{
    return current;
}

Entity ChildIterator::operator->()
{
    return current;
}

ChildIterator& ChildIterator::operator++()
{
    std::cout << "size: " << size << std::endl;
    std::cout << "Test: " << (current != Entity::INVALID ? current.GetName() : "invalid") << std::endl;
    if (index < size - 1)
    {
        
        auto& relationship = current.GetRelationship();
        if (traverseForward) current = relationship.GetNextSibling();
        else current = relationship.GetPrevSibling();
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

ChildIterator ChildIterator::operator++(int)
{
    ChildIterator tmp = *this;
    ++(*this);
    return tmp;
}

bool ChildIterator::operator==(const ChildIterator& other) const
{
    return current == other.current;
}

bool ChildIterator::operator!=(const ChildIterator& other) const
{
    return !(*this == other);
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

} // namespace velecs::ecs
