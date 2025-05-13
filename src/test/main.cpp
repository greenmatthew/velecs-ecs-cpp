#include <iostream>

#include "velecs/ecs/Entity.hpp"
#include "velecs/ecs/Component.hpp"
#include "velecs/ecs/Transform.hpp"

using namespace velecs::ecs;

int main()
{
    std::cout << "Hello World!" << std::endl;

    // entt::registry registry;

    // const auto entity = registry.create();
    // registry.destroy(entity);

    auto entity = Entity::Create();
    std::cout << "Transform: " << entity->name << std::endl;

    Transform& transform = entity->AddComponent<Transform>();
    std::cout << "\tPosition: " << transform.pos << std::endl;
    std::cout << "\tScale: " << transform.scale << std::endl;
    std::cout << "\tRotation: " << transform.rot << std::endl;

    Entity::RequestDestroy(entity);
    Entity::ProcessDestructionQueue();

    return EXIT_SUCCESS;
}
