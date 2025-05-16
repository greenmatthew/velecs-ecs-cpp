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
    inline Relationship()
        : prevSibling(Entity::INVALID), nextSibling(Entity::INVALID)
    {
        const auto owner = GetOwner<Relationship>();
        this->prevSibling = owner;
        this->nextSibling = owner;
    }

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
        return (childCount > 0) ? iterator(firstChild, childCount, true) : end();
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
        return (childCount > 0) ? iterator(firstChild.GetRelationship().prevSibling, childCount, false) : end();
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
        return parent;
    }

    void SetParent(const Entity newParent)
    {
        // If the new parent is the same then ignore.
        if (parent == newParent) return;

        Entity owner = this->GetOwner<Relationship>();

        // If there is already a parent then remove it as a child
        if (parent) parent.GetRelationship().RemoveChild(owner);

        // Assign new parent.
        parent = newParent;

        // Add child to new parent
        parent.GetRelationship().AddChild(owner);
    }

    inline Entity GetPrevSibling() const
    {
        return prevSibling;
    }

    inline Entity GetNextSibling() const
    {
        return nextSibling;
    }

    inline size_t GetChildCount() const
    {
        return childCount;
    }

    inline Entity GetFirstChild() const
    {
        return firstChild;
    }

    void AddChild(const Entity child)
    {
        auto owner = GetOwner<Relationship>();
        if (child == owner) return;

        if (GetChildCount() == 0) firstChild = child;
        else
        {
            auto first = GetFirstChild();
            auto& firstRelationship = first.GetRelationship();

            auto last = firstRelationship.GetPrevSibling();
            auto& lastRelationship = last.GetRelationship();

            firstRelationship.prevSibling = child;
            lastRelationship.nextSibling = child;

            auto& childRelationship = child.GetRelationship();
            childRelationship.parent = owner;
            childRelationship.prevSibling = last;
            childRelationship.nextSibling = first;
        }
        ++childCount;
    }

    
    bool RemoveChild(const Entity child)
    {
        if (GetOwner<Relationship>() == child) return false;

        auto first = firstChild;
        auto it = first;
        for(std::size_t i{}; i < childCount; ++i)
        {
            auto& relationship = it.GetRelationship();
            if (it == child)
            {
                auto prev = relationship.prevSibling;
                auto& prevRelationship = prev.GetRelationship();
                auto next = relationship.nextSibling;
                auto& nextRelationship = next.GetRelationship();

                prevRelationship.nextSibling = next;
                nextRelationship.prevSibling = prev;

                if (child == first)
                {
                    if (childCount == 1) firstChild = Entity::INVALID;
                    else firstChild = next;
                }

                auto& childRelationship = child.GetRelationship();
                childRelationship.parent = Entity::INVALID;
                childRelationship.prevSibling = child;
                childRelationship.nextSibling = child;

                --childCount;
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

    Entity parent{Entity::INVALID};
    Entity firstChild{Entity::INVALID};
    Entity nextSibling;
    Entity prevSibling;
    size_t childCount{0};

    // Private Methods
};

} // namespace velecs::ecs
