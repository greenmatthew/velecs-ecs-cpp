/// @file    SystemStorage.hpp
/// @author  Matthew Green
/// @date    2025-07-22 14:00:40
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#pragma once

#include <memory>
#include <typeindex>

namespace velecs::ecs {

class System;

/// @class SystemStorage
/// @brief Wrapper component that stores system instances in the ECS registry
/// @details This allows us to store different system types in a uniform way
///          while still maintaining type information for duplicate checking
class SystemStorage {
public:
    // Enums

    // Public Fields

    std::unique_ptr<System> system; /// @brief The actual system instance
    std::type_index typeIndex;      /// @brief Runtime type information for duplicate checking

    // Constructors and Destructors

    /// @brief Constructor that takes ownership of a system
    /// @param sys Unique pointer to the system instance
    /// @param type Type index for the system type
    inline SystemStorage(std::unique_ptr<System> sys, std::type_index type)
        : system(std::move(sys)), typeIndex(type) {}

    /// @brief Move constructor
    inline SystemStorage(SystemStorage&& other) noexcept
        : system(std::move(other.system)), typeIndex(other.typeIndex) {}

    /// @brief Move assignment
    SystemStorage& operator=(SystemStorage&& other) noexcept;

    // Delete copy operations since we're managing unique resources
    SystemStorage(const SystemStorage&) = delete;
    SystemStorage& operator=(const SystemStorage&) = delete;

    /// @brief Default deconstructor.
    ~SystemStorage() = default;

    // Public Methods

protected:
    // Protected Fields

    // Protected Methods

private:
    // Private Fields

    // Private Methods
};

} // namespace velecs::ecs
