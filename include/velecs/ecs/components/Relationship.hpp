/// @file    Relationship.hpp
/// @author  Matthew Green
/// @date    2025-05-15 11:16:31
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include "velecs/ecs/Component.hpp"

#include "velecs/ecs/Entity.hpp"
// #include "velecs/ecs/components/ChildIterator.hpp"

namespace velecs::ecs {

class Entity;

/// @class Relationship
/// @brief Brief description.
///
/// Rest of description.
class Relationship : public Component {
public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor.
    Relationship() = default;

    /// @brief Default deconstructor.
    ~Relationship() = default;

    // Public Methods

    /// @brief Iterator type for traversing child entities
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Entity;
        using difference_type = std::ptrdiff_t;
        using pointer = Entity;
        using reference = Entity;

        /// @brief Create a relationship iterator
        /// @param start First child's associated Entity
        /// @param size Number of children
        inline iterator(const Entity start, const size_t size, const bool traverseForward)
            : current(start), size(size), traverseForward(traverseForward) {}
        
        /// @brief Special constructor for end iterator
        inline iterator()
            : current(Entity::INVALID), size(0), index(0), traverseForward(true) {}

        velecs::ecs::Entity operator*();
        velecs::ecs::Entity operator->();
        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;

    private:
        Entity current;
        const size_t size;
        const bool traverseForward;
        size_t index{0};
    };

    /// @brief Returns an iterator to the first child entity
    /// @return Iterator to the first child, or end() if there are no children
    inline iterator begin()
    {
        return (_childCount > 0) ? iterator(_firstChild, _childCount, true) : end();
    }

    /// @brief Returns an iterator representing the end of child traversal
    /// @return End iterator with an invalid entity
    inline iterator end()
    {
        return iterator();
    }

    /// @brief Returns a reverse iterator to the last child entity
    /// @return Reverse iterator to the last child, or rend() if there are no children
    inline iterator rbegin()
    {
        return (_childCount > 0) ? iterator(_firstChild.GetRelationship()._prevSibling, _childCount, false) : end();
    }

    /// @brief Returns a reverse iterator representing the end of reverse traversal
    /// @return Reverse end iterator with an invalid entity
    inline iterator rend()
    {
        return iterator();
    }

    /// @class reverse_iterator
    /// @brief Helper class that enables reverse iteration in range-based for loops
    /// @details This wrapper provides begin/end methods that return reverse iterators,
    ///          allowing for convenient reverse traversal of child entities.
    class reverse_iterator {
    public:
        reverse_iterator(Relationship& rel) : rel(rel) {}
        
        iterator begin() { return rel.rbegin(); }
        iterator end() { return rel.rend(); }
    
    private:
        Relationship& rel;
    };

    /// @brief Creates a helper object for reverse iteration of children
    /// @return An object that enables reverse traversal in range-based for loops
    /// @details Use with range-based for loops: for (auto& child : relationship.Reverse())
    reverse_iterator Reverse()
    {
        return reverse_iterator(*this);
    }

    inline Entity GetParent() const
    {
        return _parent;
    }

    void SetParent(const Entity newParent)
    {
        // If the new parent is the same then ignore.
        if (_parent == newParent) return;

        const auto& owner = GetOwner();

        // If there is already a parent then remove it as a child
        if (_parent) _parent.GetRelationship().RemoveChild(owner);

        // Assign new parent.
        _parent = newParent;

        // Add child to new parent
        _parent.GetRelationship().AddChild(owner);
    }

    inline Entity GetPrevSibling() const
    {
        return _prevSibling;
    }

    inline Entity GetNextSibling() const
    {
        return _nextSibling;
    }

    inline size_t GetChildCount() const
    {
        return _childCount;
    }

    inline Entity GetFirstChild() const
    {
        return _firstChild;
    }

    void AddChild(const Entity child)
    {
        const auto& owner = GetOwner();
        if (child == owner) return;

        auto& childRelationship = child.GetRelationship();
        childRelationship._parent = owner;

        if (_childCount == 0)
        {
            _firstChild = child;

            childRelationship._prevSibling = Entity::INVALID;
            childRelationship._nextSibling = Entity::INVALID;
        }
        else if (_childCount == 1)
        {
            auto first = GetFirstChild();
            auto& firstRelationship = first.GetRelationship();

            firstRelationship._nextSibling = child;
            firstRelationship._prevSibling = child;

            childRelationship._prevSibling = first;
            childRelationship._nextSibling = first;
        }
        else
        {
            auto first = GetFirstChild();
            auto& firstRelationship = first.GetRelationship();

            auto last = firstRelationship.GetPrevSibling();
            auto& lastRelationship = last.GetRelationship();

            firstRelationship._prevSibling = child;
            lastRelationship._nextSibling = child;
            
            childRelationship._prevSibling = last;
            childRelationship._nextSibling = first;
        }
        ++_childCount;
    }

    
    bool RemoveChild(const Entity child)
    {
        const auto& owner = GetOwner();
        if (owner == child) return false;

        auto first = _firstChild;
        auto it = first;
        for(std::size_t i{}; i < _childCount; ++i)
        {
            auto& relationship = it.GetRelationship();
            if (it == child)
            {
                if (_childCount == 1)
                {
                    // Only one child - simple case
                    _firstChild = Entity::INVALID;
                }
                else if (_childCount == 2)
                {
                    // Two children - the remaining child should have INVALID siblings
                    auto other = (child == first) ? relationship._nextSibling : first;
                    auto& otherRelationship = other.GetRelationship();
                    otherRelationship._prevSibling = Entity::INVALID;
                    otherRelationship._nextSibling = Entity::INVALID;
                    _firstChild = other;
                }
                else
                {
                    // Three or more children - update the circular list
                    auto prev = relationship._prevSibling;
                    auto next = relationship._nextSibling;
                    
                    // Both prev and next should be valid in a 3+ child scenario
                    if (prev && next)
                    {
                        auto& prevRelationship = prev.GetRelationship();
                        auto& nextRelationship = next.GetRelationship();
                        
                        prevRelationship._nextSibling = next;
                        nextRelationship._prevSibling = prev;
                        
                        // Update firstChild if we're removing the first child
                        if (child == first)
                        {
                            _firstChild = next;
                        }
                    }
                }

                // Clean up the removed child's relationship
                auto& childRelationship = child.GetRelationship();
                childRelationship._parent = Entity::INVALID;
                childRelationship._prevSibling = Entity::INVALID;
                childRelationship._nextSibling = Entity::INVALID;

                --_childCount;
                return true;
            }
            it = relationship.GetNextSibling();
        }

        return false;
    }

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    Entity _parent{Entity::INVALID};
    Entity _firstChild{Entity::INVALID};
    Entity _nextSibling{Entity::INVALID};
    Entity _prevSibling{Entity::INVALID};
    size_t _childCount{0};

    // Private Methods
};

} // namespace velecs::ecs
