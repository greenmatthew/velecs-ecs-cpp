/// @file    SceneNode.hpp
/// @author  Matthew Green
/// @date    2025-05-13 14:13:34
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include <string>
#include <vector>

namespace velecs::ecs {

template <typename T> class SceneNodeRef;

/// @class SceneNode
/// @brief Brief description.
///
/// Rest of description.
class SceneNode {
    template <typename T>
    friend class SceneNodeRef;

public:
    // Enums

    // Public Fields

    std::string name = "Scene Node";

    // Constructors and Destructors

    /// @brief Default constructor.
    SceneNode() = default;

    /// @brief Default deconstructor.
    ~SceneNode() = default;

    // Public Methods

    static SceneNode* Create();
    static void RequestDestroy(SceneNode* nodePtr);

    virtual void OnCreate() {}
    virtual void OnDestroy() {}

    void AddChild(SceneNode* child);
    void RemoveChild(SceneNode* child);
    SceneNode* GetParent() const;
    const std::vector<SceneNode*>& GetChildren() const;

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    SceneNode* parent = nullptr;
    std::vector<SceneNode*> children;

    // Private Methods
};

} // namespace velecs::ecs
