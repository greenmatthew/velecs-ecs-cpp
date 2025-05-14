#include <iostream>

#include "velecs/ecs/Common.hpp"

using namespace velecs::ecs;
using namespace velecs::math;

int main()
{
    Entity parent = Entity::Create();
    Transform& parentTransform = parent.GetTransform();
    std::cout << "Transform: " << parent.GetName() << std::endl;
    std::cout << "\tPosition: " << parentTransform.GetPos() << std::endl;
    std::cout << "\tScale: " << parentTransform.GetScale() << std::endl;
    std::cout << "\tRotation: " << parentTransform.GetEulerAnglesDeg() << std::endl;
    std::cout << "\tModel Matrix:\n" << parentTransform.GetModelMatrix() << std::endl;

    Entity child = Entity::Create();
    Transform& childTransform = child.GetTransform();
    childTransform.SetParent(parent);
    childTransform.SetPos(Vec3::FORWARD * 10);
    std::cout << "Transform: " << child.GetName() << std::endl;
    std::cout << "\tParent:" << (childTransform.GetParent() ? childTransform.GetParent().GetName() : "(n/a)") << '\n' << std::endl;
    std::cout << "\tPosition: " << childTransform.GetPos() << std::endl;
    std::cout << "\tScale: " << childTransform.GetScale() << std::endl;
    std::cout << "\tRotation: " << childTransform.GetEulerAnglesDeg() << std::endl;
    std::cout << "\tModel Matrix:\n" << childTransform.GetModelMatrix() << std::endl;

    Entity::RequestDestroy(parent);
    Entity::ProcessDestructionQueue();

    return EXIT_SUCCESS;
}
