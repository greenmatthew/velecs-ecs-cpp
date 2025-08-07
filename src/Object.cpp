/// @file    Object.cpp
/// @author  Matthew Green
/// @date    2025-08-07 16:01:44
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#include "velecs/ecs/Object.hpp"

namespace velecs::ecs {

// Public Fields

const Object Object::INVALID{};

// Constructors and Destructors

// Public Methods

bool Object::operator==(const Object& other) const
{
    return _registry == other._registry && _handle == other._handle;
}

bool Object::operator!=(const Object& other) const
{
    return !(*this == other);
}

size_t Object::GetHashCode() const
{
    // Combine registry pointer and entity handle for unique hash
    size_t h1 = std::hash<entt::registry*>{}(_registry);
    size_t h2 = std::hash<entt::entity>{}(_handle);
    
    // Use a simple hash combination technique
    return h1 ^ (h2 << 1);
}

bool Object::IsValid() const
{
    return _handle != entt::null &&
        _registry != nullptr &&
        _registry->valid(_handle);
}

void Object::RequestDestroy()
{
    if (IsValid()) _registry->emplace_or_replace<RequestDestructionTag>(_handle);
}

const std::string& Object::GetName() const
{
    return _name;
}

void Object::SetName(const std::string& name)
{
    _name = name;
}

// Protected Fields

// Protected Methods

Object::Object(entt::registry* registry, entt::entity handle, const std::string& name)
    : _registry(registry), _handle(handle), _name(name)
{
    // Validate that the handle is actually valid in the registry
    if (!IsValid())
    {
        // Reset to invalid state if registry is null or handle is not valid
        Reset();
    }
}

// Private Fields

// Private Methods

void Object::Destroy()
{
    if (IsValid()) _registry->destroy(_handle);
    Reset();
}

void Object::Reset()
{
    _registry = nullptr;
    _handle = entt::null;
    _name = "Invalid Object";
}

} // namespace velecs::ecs
