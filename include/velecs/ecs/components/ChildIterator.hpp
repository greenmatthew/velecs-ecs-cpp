/// @file    ChildIterator.hpp
/// @author  Matthew Green
/// @date    2025-05-15 15:23:29
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include "velecs/ecs/Entity.hpp"

#include <iterator>

namespace velecs::ecs {

class Relationship;

/// @class ChildIterator
/// @brief Brief description.
///
/// Rest of description.
class ChildIterator {
public:
    // Iterator traits
    using iterator_category = std::forward_iterator_tag;
    using value_type = Entity;
    using difference_type = std::ptrdiff_t;
    using pointer = Entity;
    using reference = Entity;

    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Create a relationship iterator
    /// @param start First child's associated Entity
    /// @param size Number of children
    inline ChildIterator(const Entity start, const size_t size, const bool traverseForward)
        : current(start), size(size), traverseForward(traverseForward) {}
    
    /// @brief Special constructor for end iterator
    inline ChildIterator()
        : current(Entity::INVALID), size(0), index(0), traverseForward(true) {}

    // Public Methods

    Entity operator*();
    Entity operator->();
    ChildIterator& operator++();
    ChildIterator operator++(int);
    bool operator==(const ChildIterator& other) const;
    bool operator!=(const ChildIterator& other) const;

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    Entity current;
    const size_t size;
    const bool traverseForward;
    size_t index{0};

    // Private Methods
};

} // namespace velecs::ecs
