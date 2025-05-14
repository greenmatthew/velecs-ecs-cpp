#include <iostream>

#include "velecs/ecs/Common.hpp"

using namespace velecs::ecs;
using namespace velecs::math;

int main()
{
    Entity parent = Entity::Create();
    auto& parentHierarchy = parent.AddComponent<Hierarchy>();
    Name* name = nullptr;
    if (parent.TryGetComponent<Name>(name)) name->SetName("parent");
    std::cout << "Transform: " << (parent.TryGetComponent<Name>(name) ? name->GetName() : "Unknown") << std::endl;
    Transform& parentTransform = parent.AddComponent<Transform>();
    std::cout << "\tPosition: " << parentTransform.GetPos() << std::endl;
    std::cout << "\tScale: " << parentTransform.GetScale() << std::endl;
    std::cout << "\tRotation: " << parentTransform.GetEulerAnglesDeg() << std::endl;
    std::cout << "\tModel Matrix:\n" << parentTransform.GetModelMatrix() << std::endl;

    Entity child = Entity::Create();
    auto& childHierarchy = child.AddComponent<Hierarchy>();
    childHierarchy.SetParent(parent);
    if (child.TryGetComponent<Name>(name)) name->SetName("child");
    std::cout << "Transform: " << (child.TryGetComponent<Name>(name) ? name->GetName() : "Unknown") << std::endl;
    Transform& childTransform = child.AddComponent<Transform>();
    childTransform.SetPos(Vec3::FORWARD * 10);
    std::cout << "\tPosition: " << childTransform.GetPos() << std::endl;
    std::cout << "\tScale: " << childTransform.GetScale() << std::endl;
    std::cout << "\tRotation: " << childTransform.GetEulerAnglesDeg() << std::endl;
    std::cout << "\tModel Matrix:\n" << childTransform.GetModelMatrix() << std::endl;

    Entity::RequestDestroy(parent);
    Entity::ProcessDestructionQueue();

    return EXIT_SUCCESS;
}
