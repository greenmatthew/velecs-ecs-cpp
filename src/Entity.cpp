/// @file    Entity.cpp
/// @author  Matthew Green
/// @date    2025-05-12 16:24:39
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#include "velecs/ecs/Entity.hpp"
#include "velecs/ecs/EntityBuilder.hpp"

#include "velecs/ecs/tags/DestroyTag.hpp"

#include "velecs/ecs/components/Name.hpp"
#include "velecs/ecs/components/Transform.hpp"

#include <memory>
#include <string>
#include <algorithm>

namespace velecs::ecs {

// Public Fields

const Entity Entity::INVALID;

// Constructors and Destructors

EntityBuilder Entity::Create()
{
    return EntityBuilder();
}

// Public Methods

std::string Entity::GetName() const
{
    return GetComponent<Name>().GetName();
}

void Entity::SetName(const std::string& newName)
{
    GetComponent<Name>().SetName(newName);
}

Transform& Entity::GetTransform() const
{
    return GetComponent<Transform>();
}

void Entity::RequestDestroy(Entity entity)
{
    if (entity) entity.AddTag<DestroyTag>();
}

void Entity::ProcessDestructionQueue()
{
    // Create a view of all entities with the DestroyTag
    auto view = Registry::Get().view<DestroyTag>();
    
    // Store the entities in a temporary vector
    // This is necessary because we'll be modifying the registry during iteration
    std::vector<Entity> toDestroy;
    for (auto handle : view)
    {
        toDestroy.push_back(Entity(handle));
    }
    
    // Now process each entity for destruction
    for (auto entity : toDestroy)
    {
        // Ensure entity is still valid before destroying it
        if (entity) entity.Destroy(true);
    }
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

void Entity::Destroy(bool removeParent) const
{
    auto& transform = GetTransform();
    const size_t childCount = transform.GetChildCount();
    if (childCount > 0)
    {
        const auto childCountStr = std::to_string(childCount) + (childCount == 1 ? " child" : " children");
        std::cout << "Destroying the " << childCountStr << " of '" << GetName() << "':" << std::endl;
        for (auto it = transform.rbegin(); it != transform.rend(); ++it)
        {
            const Entity& child = *it;
            if (child) child.Destroy(false);
            else std::cout << "ERROR: child entity is not valid!" << std::endl;
        }
    }

    if (removeParent) transform.TrySetParent(Entity::INVALID);

    std::cout << "Destroying '" << GetName() << "'!" << std::endl;
    GetRegistry().destroy(_handle);
}

} // namespace velecs::ecs
