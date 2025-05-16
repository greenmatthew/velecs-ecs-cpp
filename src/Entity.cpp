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
#include "velecs/ecs/components/Name.hpp"
#include "velecs/ecs/components/Relationship.hpp"
#include "velecs/ecs/components/Transform.hpp"

#include <memory>
#include <string>

namespace velecs::ecs {

// Public Fields

const Entity Entity::INVALID;

// Constructors and Destructors

// Public Methods

Entity Entity::Create()
{
    entt::entity handle = registry.create();
    Entity result = Entity(handle);
    result.AddComponent<Name>("Entity");
    result.AddComponent<Relationship>();
    result.AddComponent<Transform>();
    return result;
}

std::string Entity::GetName() const
{
    return GetComponent<Name>().GetName();
}

void Entity::SetName(const std::string& newName)
{
    GetComponent<Name>().SetName(newName);
}

Relationship& Entity::GetRelationship() const
{
    return GetComponent<Relationship>();
}

Transform& Entity::GetTransform() const
{
    return GetComponent<Transform>();
}

void Entity::RequestDestroy(Entity entity)
{
    if (entity)
    {
        destructionQueue.push_back(entity);
    }
}

void Entity::ProcessDestructionQueue()
{
    std::vector<Entity> currentQueue = std::move(destructionQueue);
    destructionQueue.clear();  // Clear it now in case more entities get queued during processing

    for (auto& entity : currentQueue)
    {
        // Ensure entity still is valid and exists
        // i.e. in case entity was a child of something already destroyed.
        if (entity) entity.Destroy(true);
    }
}

// Protected Fields

// Protected Methods

// Private Fields

entt::registry Entity::registry;
std::vector<Entity> Entity::destructionQueue;

// Private Methods

void Entity::Destroy(bool removeParent) const
{
    auto& relationship = GetRelationship();
    const size_t childCount = relationship.GetChildCount();
    if (childCount > 0)
    {
        const auto childCountStr = std::to_string(childCount) + (childCount == 1 ? " child" : " children");
        std::cout << "Destroying the " << childCountStr << " of '" << GetName() << "':" << std::endl;
        for (const Entity& child : relationship.Reverse())
        {
            if (child) child.Destroy(false);
            else std::cout << "ERROR: child entity is not valid!" << std::endl;
        }
    }

    if (removeParent) relationship.SetParent(Entity::INVALID);

    std::cout << "Destroying '" << GetName() << "'!" << std::endl;
    registry.destroy(handle);
}

} // namespace velecs::ecs
