/// @file    Registry.hpp
/// @author  Matthew Green
/// @date    2025-07-23 11:54:54
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include <entt/entt.hpp>

namespace velecs::ecs {

/// @class Registry
/// @brief Brief description.
///
/// Rest of description.
class Registry {
public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor.
    Registry() = default;

    /// @brief Default deconstructor.
    ~Registry() = default;

    // Public Methods

    inline static entt::registry& Get() { return _registry; }

    inline static entt::entity CreateEntity() { return _registry.create(); }

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    static entt::registry _registry;

    // Private Methods
};

} // namespace velecs::ecs
