/// @file    IComponentContainer.hpp
/// @author  Matthew Green
/// @date    2025-05-13 14:19:43
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include "velecs/ecs/TypeConstraints.hpp"

namespace velecs::ecs {

/// @class IComponentContainer
/// @brief Brief description.
///
/// Rest of description.
class IComponentContainer {
public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor.
    IComponentContainer() = default;

    /// @brief Default deconstructor.
    ~IComponentContainer() = default;

    // Public Methods

    template<typename T, typename = IsComponent<T>>
    inline T& AddComponent()
    {
        return registry.emplace<T>(handle);
    }

    // template<typename T, IsComponent<T>, typename... Args>
    // void AddComponent(Args &&...args)
    // {
    //     auto& comp = registry.emplace<T>(handle, std::forward<Args>(args)...);
    //     // return comp;
    // }

    template<typename T, typename = IsComponent<T>>
    void RemoveComponent();

    template<typename T, typename = IsComponent<T>>
    void TryGetComponent();

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    // Private Methods
};

} // namespace velecs::ecs
