#pragma once

#include "velecs/ecs/Component.hpp"
#include "velecs/ecs/Entity.hpp"

#include <velecs/common/Exceptions.hpp>

#include <velecs/math/Vec3.hpp>
#include <velecs/math/Quat.hpp>
#include <velecs/math/Mat4.hpp>

#include <stack>
#include <queue>
#include <iterator>
#include <vector>

namespace velecs::ecs {

enum class TraversalOrder;

template<TraversalOrder Order>
class TraversalRange;

/// @class Transform
/// @brief Component representing spatial transformation and hierarchical relationships.
/// @details Combines spatial transformation (position, rotation, scale) with scene graph 
///          hierarchy management. Provides cached matrix calculations for efficient
///          rendering transformations.
class Transform : public Component {
public:
    using Vec3 = velecs::math::Vec3;
    using Quat = velecs::math::Quat;
    using Mat4 = velecs::math::Mat4;

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor.
    Transform() = default;

    /// @brief Default deconstructor.
    ~Transform() = default;

    // Public Methods

    /// @brief Gets the local position of this transform.
    /// @return Local position vector relative to parent (or world if no parent).
    Vec3 GetPos() const;

    /// @brief Sets the local position of this transform.
    /// @param newPos New local position vector.
    /// @details Marks transform matrices as dirty and updates all children.
    void SetPos(const Vec3& newPos);

    /// @brief Gets the local scale of this transform.
    /// @return Local scale vector relative to parent.
    Vec3 GetScale() const;

    /// @brief Sets the local scale of this transform.
    /// @param newScale New local scale vector.
    /// @details Marks transform matrices as dirty and updates all children.
    void SetScale(const Vec3& newScale);

    /// @brief Gets the local rotation as Euler angles in radians.
    /// @return Local rotation as Euler angles (pitch, yaw, roll) in radians.
    Vec3 GetEulerAnglesRad() const;

    /// @brief Gets the local rotation as Euler angles in degrees.
    /// @return Local rotation as Euler angles (pitch, yaw, roll) in degrees.
    Vec3 GetEulerAnglesDeg() const;

    /// @brief Sets the local rotation using Euler angles in radians.
    /// @param newAngles New rotation as Euler angles (pitch, yaw, roll) in radians.
    /// @details Marks transform matrices as dirty and updates all children.
    void SetEulerAnglesRad(const Vec3& newAngles);

    /// @brief Sets the local rotation using Euler angles in degrees.
    /// @param newAnglesDeg New rotation as Euler angles (pitch, yaw, roll) in degrees.
    /// @details Marks transform matrices as dirty and updates all children.
    void SetEulerAnglesDeg(const Vec3& newAnglesDeg);

    /// @brief Gets the local rotation as a quaternion.
    /// @return Local rotation quaternion relative to parent.
    Quat GetRot() const;

    /// @brief Sets the local rotation using a quaternion.
    /// @param newRot New rotation quaternion.
    /// @details Marks transform matrices as dirty and updates all children.
    void SetRot(const Quat& newRot);

    // ========== Matrix Methods ==========

    /// @brief Gets the local-to-parent transformation matrix.
    /// @return 4x4 matrix representing local transformation (TRS).
    /// @details Matrix is cached and only recalculated when transform data changes.
    Mat4 GetModelMatrix() const;

    /// @brief Gets the local-to-world transformation matrix.
    /// @return 4x4 matrix representing world transformation.
    /// @details Combines this transform with all parent transforms up to root.
    ///          Matrix is cached and only recalculated when hierarchy changes.
    Mat4 GetWorldMatrix() const;

    // ========== Parent Management ==========

    inline bool HasParent(const Entity* const parent) const { return _parent == parent; }

    /// @brief Gets the parent entity of this transform.
    /// @return Parent entity, or Entity::INVALID if this is a root transform.
    inline Entity* GetParent() const { return _parent; }

    /// @brief Attempts to set the parent of this transform.
    /// @param newParent New parent entity, or Entity::INVALID to make this a root transform.
    /// @return True if parent was successfully changed, false if operation failed or no change was needed.
    /// @details Automatically handles bidirectional relationship updates. Removes this
    ///          transform from old parent's children list and adds to new parent's list.
    ///          Marks world matrix as dirty for this transform and all children.
    ///          Fails if it would create a cycle in the hierarchy.
    bool TrySetParent(Entity* const newParent);

    // ========== Child Management ==========

    /// @brief Checks if the specified entity is a direct child of this transform.
    /// @param child Entity to check for.
    /// @return True if child is a direct child, false otherwise.
    bool HasChild(const Entity* const child) const;

    /// @brief Attempts to get a child by index.
    /// @param index Zero-based index of child to retrieve.
    /// @return Child entity at index, or Entity::INVALID if index is out of range.
    Entity* TryGetChild(const size_t index) const;

    /// @brief Gets read-only access to all direct children.
    /// @return Const reference to vector containing all child entities.
    /// @details Use for iteration or when you need the full children list.
    inline const std::vector<Entity*>& GetChildren() const { return _children; }

    /// @brief Gets the number of direct children.
    /// @return Number of direct child entities.
    inline size_t GetChildCount() const { return _children.size(); }

    /// @brief Attempts to add a child to this transform.
    /// @param child Entity to add as child.
    /// @return True if child was successfully added, false if operation failed.
    /// @details Fails if child is invalid, child is this entity (self-parenting),
    ///          or if adding would create a cycle. Automatically removes child
    ///          from its previous parent.
    bool TryAddChild(Entity* const child);

    /// @brief Attempts to remove a child from this transform.
    /// @param child Entity to remove from children.
    /// @return True if child was found and removed, false if child was not found.
    /// @details Sets the child's parent to a nullptr.
    bool TryRemoveChild(Entity* const child);

    /// @brief Attempts to remove a child by index.
    /// @param index Zero-based index of child to remove.
    /// @return True if child was removed, false if index was out of range.
    /// @details Sets the child's parent to a nullptr.
    bool TryRemoveChild(const size_t index);

    // ========== Sibling Management ==========

    /// @brief Gets this transform's index among its siblings.
    /// @return Zero-based index among siblings, or 0 if this transform has no parent.
    /// @details Index represents order in parent's children list.
    size_t GetSiblingIndex() const;

    /// @brief Attempts to set this transform's index among its siblings.
    /// @param index New zero-based index position.
    /// @return True if sibling index was successfully changed, false if this transform has no parent.
    /// @details Clamps index to valid range. Throws std::runtime_error if hierarchy corruption is detected.
    /// @throws std::runtime_error If entity claims to have a parent but isn't in parent's children list.
    bool TrySetSiblingIndex(const size_t index);

    /// @brief Attempts to move this transform to be the first child of its parent.
    /// @return True if successfully moved to first position, false if this transform has no parent.
    /// @details Equivalent to TrySetSiblingIndex(0).
    /// @throws std::runtime_error If hierarchy corruption is detected during the operation.
    bool TrySetAsFirstSibling();

    /// @brief Attempts to move this transform to be the last child of its parent.
    /// @return True if successfully moved to last position, false if this transform has no parent.
    /// @details Moves to end of parent's children list.
    /// @throws std::runtime_error If hierarchy corruption is detected during the operation.
    bool TrySetAsLastSibling();

    // ========== Relationship Queries ==========

    /// @brief Checks if this transform is a direct child of the specified parent.
    /// @param parent Entity to check against.
    /// @return True if this transform's parent is the specified entity.
    bool IsChildOf(const Entity* const parent) const;

    /// @brief Checks if this transform is anywhere in the hierarchy below ancestor.
    /// @param ancestor Entity to check against.
    /// @return True if ancestor is found anywhere up the parent chain.
    /// @details Performs recursive search up the hierarchy tree.
    bool IsDescendantOf(const Entity* const ancestor) const;

    /// @brief Checks if this transform is an ancestor of the specified descendant.
    /// @param descendant Entity to check against.
    /// @return True if descendant is found anywhere in this transform's hierarchy.
    /// @details More efficient than manually traversing children.
    bool IsAncestorOf(const Entity* const descendant) const;

    /// @brief Gets the root transform of this hierarchy.
    /// @return The topmost ancestor, or this transform if it has no parent.
    /// @details Traverses up the parent chain until reaching a transform with no parent.
    Entity* GetRoot() const;

    // ========== Iterator Support ==========

    /// @brief Iterator type for range-based loops over children.
    /// @details Allows using range-based for loops: for (auto child : transform) { ... }
    using iterator = std::vector<Entity*>::const_iterator;

    /// @brief Gets iterator to first child.
    /// @return Const iterator pointing to first child entity.
    inline iterator begin() const { return _children.begin(); }

    /// @brief Gets iterator past last child.
    /// @return Const iterator pointing past the last child entity.
    inline iterator end() const { return _children.end(); }

    /// @brief Reverse iterator type for iterating children in reverse order.
    using reverse_iterator = std::vector<Entity*>::const_reverse_iterator;

    /// @brief Gets reverse iterator to last child (for iterating backwards).
    /// @return Reverse iterator pointing to last child entity.
    reverse_iterator rbegin() const { return _children.rbegin(); }

    /// @brief Gets reverse iterator past first child (reverse end sentinel).
    /// @return Reverse iterator representing reverse iteration end.
    reverse_iterator rend() const { return _children.rend(); }

    /// @brief Creates a traversal range for iterating through the transform hierarchy
    /// @tparam Order The traversal order to use
    /// @return A range object that can be used in range-based for loops
    template<TraversalOrder Order>
    TraversalRange<Order> Traverse() const
    {
        return TraversalRange<Order>(GetOwner());
    }

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    Vec3 pos{Vec3::ZERO};                        /// @brief Local position relative to parent
    Vec3 scale{Vec3::ONE};                       /// @brief Local scale relative to parent  
    Quat rot{Quat::IDENTITY};                    /// @brief Local rotation relative to parent

    Entity* _parent{nullptr};                    /// @brief Parent entity (nullptr if root)
    std::vector<Entity*> _children;              /// @brief List of direct child entities

    mutable bool isModelDirty{true};             /// @brief Flag indicating model matrix needs recalculation
    mutable Mat4 cachedModelMat{Mat4::IDENTITY}; /// @brief Cached local-to-parent transformation matrix

    mutable bool isWorldDirty{true};             /// @brief Flag indicating world matrix needs recalculation
    mutable Mat4 cachedWorldMat{Mat4::IDENTITY}; /// @brief Cached local-to-world transformation matrix

    // Private Methods

    /// @brief Calculates the local-to-parent transformation matrix.
    /// @return 4x4 TRS matrix combining translation, rotation, and scale.
    math::Mat4 CalculateModel() const;

    /// @brief Calculates the local-to-world transformation matrix.
    /// @return 4x4 matrix representing world transformation.
    /// @details Multiplies parent's world matrix with this transform's model matrix.
    math::Mat4 CalculateWorld() const;

    /// @brief Marks world matrix as dirty and propagates to all children.
    /// @details Called when this transform's world position might have changed.
    void SetWorldDirty();

    /// @brief Marks both model and world matrices as dirty.
    /// @details Called when this transform's local properties change.
    void SetDirty();
};

/// @enum TraversalOrder
/// @brief Specifies the order for tree traversal operations
enum class TraversalOrder {
    PreOrder,          ///< @brief Visit root first, then children (depth-first)
    InOrder,           ///< @brief Visit left subtree, root, then right subtree (depth-first)
    PostOrder,         ///< @brief Visit children first, then root (depth-first)
    LevelOrder,        ///< @brief Visit nodes level by level, top to bottom (breadth-first)
    ReverseLevelOrder, ///< @brief Visit nodes level by level, bottom to top (depth-first)
};

/// @class TraversalIterator
/// @brief Lazy iterator for transform hierarchy traversal using template specialization
/// @tparam Order The traversal order to use
template<TraversalOrder Order>
class TraversalIterator {
public:
    // STL iterator traits
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::pair<Entity*, Transform&>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    /// @brief Default constructor.
    inline TraversalIterator() : _isEnd(true) {}

    explicit TraversalIterator(Entity* root)
        : _isEnd(false)
    {
        if (root == nullptr)
        {
            _isEnd = true;
            return;
        }
        Initialize(root);
    }

    /// @brief Default deconstructor.
    ~TraversalIterator() = default;

    value_type operator*() const
    {
        return std::make_pair(_current, std::ref(_current->GetTransform()));
    }

    value_type operator->() const
    {
        return *this;
    }

    TraversalIterator& operator++()
    {
        Advance();
        return *this;
    }

    TraversalIterator& operator++(int)
    {
        TraversalIterator& tmp = *this;
        ++(*this);
        return tmp;
    }
    
    bool operator==(const TraversalIterator& other) const
    {
        return _isEnd == other._isEnd && (_isEnd || _current == other._current);
    }

    bool operator!=(const TraversalIterator& other) const {
        return !(*this == other);
    }

private:

    Entity* _current{nullptr};
    bool _isEnd{true};
    std::stack<Entity*> _stack;
    std::unordered_set<Entity*> _visitedParents; // For post-order traversal

    void Initialize(Entity* const root);
    void Advance();
    void AdvanceToNextPostOrderNode();
    void AdvanceToNextInOrderNode();
};

/// @class TraversalRange
/// @brief Range wrapper for transform hierarchy traversal
/// @tparam Order The traversal order to use
template<TraversalOrder Order>
class TraversalRange {
public:
    TraversalRange(Entity* root) : _root(root) {}
    TraversalIterator<Order> begin() const { return TraversalIterator<Order>(_root); }
    TraversalIterator<Order> end() const { return TraversalIterator<Order>(); }

private:
    Entity* _root;
};

// Template specialization for PreOrder
template<>
inline void TraversalIterator<TraversalOrder::PreOrder>::Initialize(Entity* const root)
{
    _stack.push(root);
    Advance(); // Position at first element
}

template<>
inline void TraversalIterator<TraversalOrder::PreOrder>::Advance()
{
    if (_stack.empty())
    {
        _isEnd = true;
        return;
    }
    
    // Pop next entity to visit
    _current = _stack.top();
    _stack.pop();
    
    // Push children in reverse order so left-most child is processed first
    const auto& children = _current->GetTransform().GetChildren();
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
        if (*it != nullptr)
        {
            _stack.push(*it);
        }
    }
}

// Template specialization for PostOrder
template<>
inline void TraversalIterator<TraversalOrder::PostOrder>::Initialize(Entity* const root)
{
    _visitedParents.clear();
    _stack.push(root);
    AdvanceToNextPostOrderNode();
}

template<>
inline void TraversalIterator<TraversalOrder::PostOrder>::Advance()
{
    if (_stack.empty())
    {
        _isEnd = true;
        return;
    }
    
    // Remove the current node we just visited and advance
    _stack.pop();
    AdvanceToNextPostOrderNode();
}

// Helper method for post-order traversal - needs to be added to class
template<>
inline void TraversalIterator<TraversalOrder::PostOrder>::AdvanceToNextPostOrderNode()
{
    while (!_stack.empty())
    {
        Entity* top = _stack.top();
        
        // Check if we've already processed this node's children
        if (_visitedParents.find(top) != _visitedParents.end())
        {
            // All children have been processed, this node is ready to visit
            _current = top;
            return;
        }
        
        // Mark this node as having its children processed
        _visitedParents.insert(top);
        
        // Add children to stack (in reverse order so leftmost is processed first)
        const auto& children = top->GetTransform().GetChildren();
        bool hasValidChildren = false;
        
        for (auto it = children.rbegin(); it != children.rend(); ++it)
        {
            if (*it != nullptr)
            {
                _stack.push(*it);
                hasValidChildren = true;
            }
        }
        
        // If no valid children, this node is ready to visit immediately
        if (!hasValidChildren)
        {
            _current = top;
            return;
        }
    }
    
    // Stack is empty, traversal complete
    _isEnd = true;
}

} // namespace velecs::ecs
