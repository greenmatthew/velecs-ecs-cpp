/// @file    Name.hpp
/// @author  Matthew Green
/// @date    2025-05-13 17:54:40
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include "velecs/ecs/Component.hpp"

#include <iostream>

namespace velecs::ecs {

/// @class Name
/// @brief Brief description.
///
/// Rest of description.
class Name : public Component {
public:
    // Enums

    // Public Fields

    // Constructors and Destructors

    /// @brief Default constructor.
    inline Name()
        : name("Entity") {}

    inline Name(const std::string& name)
        : name(name) {}

    /// @brief Default deconstructor.
    ~Name() = default;

    // Public Methods

    inline const std::string& GetName() const
    {
        return name;
    }

    inline void SetName(const std::string& newName)
    {
        name = newName;
    }

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    std::string name;

    // Private Methods
};

} // namespace velecs::ecs
