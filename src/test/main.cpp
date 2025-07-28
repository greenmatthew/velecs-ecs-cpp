#include <iostream>

#include "velecs/ecs/Common.hpp"
using namespace velecs::ecs;

#include <velecs/math/Vec3.hpp>
using namespace velecs::math;

#include <stdexcept>

class MainScene : public Scene
{
public:
    MainScene::MainScene(const std::string& name)
        : Scene(name) {}

    MainScene::MainScene(const std::string& name, Scene::ConstructorKey key)
        : Scene(name, key) {}

    void OnEnter()
    {
        std::cout << "Calling OnEnter() on Scene." << std::endl;
    }
};

class Velocity : public Component
{
public:
    Vec3 vel{Vec3::ZERO};
};

// class MoveSystem : public System
// {
// public:
//     virtual void Update(float deltaTime) override
//     {
//         auto& registry = Registry::Get();
//         auto view = registry.view<Transform, Velocity>();
//         view.each([deltaTime](auto entity, auto& transform, auto& velocity){
//             std::cout << "Updating position" << std::endl;
//             auto pos = transform.GetPos();
//             pos += deltaTime * velocity.vel;
//             transform.SetPos(pos);
//         });
//     }
// };

int main()
{
    try
    {
        const auto mainSceneUPtr = std::make_unique<MainScene>("Main Scene");
        const auto mainScene = mainSceneUPtr.get();

        Entity parent = Entity::Create(mainScene)
            .WithName("Parent Entity")
            .WithPos(Vec3::ZERO)
            ;
        Transform& parentTransform = parent.GetTransform();
        std::cout << "Transform: " << parent.GetName() << std::endl;
        std::cout << "\tPosition: " << parentTransform.GetPos() << std::endl;
        std::cout << "\tScale: " << parentTransform.GetScale() << std::endl;
        std::cout << "\tRotation: " << parentTransform.GetEulerAnglesDeg() << std::endl;
        std::cout << "\tModel Matrix:\n" << parentTransform.GetModelMatrix() << std::endl;
        std::cout << "\tWorld Matrix:\n" << parentTransform.GetWorldMatrix() << std::endl;

        Entity child = Entity::Create(mainScene)
            .WithName("Child Entity")
            .WithParent(parent)
            .WithPos(Vec3::BACKWARD * 10)
            ;
        Transform& childTransform = child.GetTransform();
        std::cout << "Transform: " << child.GetName() << std::endl;
        std::cout << "\tParent:" << (childTransform.GetParent() ? childTransform.GetParent().GetName() : "(n/a)") << '\n' << std::endl;
        std::cout << "\tPosition: " << childTransform.GetPos() << std::endl;
        std::cout << "\tScale: " << childTransform.GetScale() << std::endl;
        std::cout << "\tRotation: " << childTransform.GetEulerAnglesDeg() << std::endl;
        std::cout << "\tModel Matrix:\n" << childTransform.GetModelMatrix() << std::endl;
        std::cout << "\tWorld Matrix:\n" << childTransform.GetWorldMatrix() << std::endl;

        
        std::cout << "Parent pos: " << parentTransform.GetPos() << std::endl;
        auto& vel = parent.AddComponent<Velocity>();
        vel.vel = Vec3::RIGHT;
        // std::cout << "Successfully added 'MoveSystem': " << std::boolalpha << System::TryAddSystem<MoveSystem>() << std::endl;
        // std::cout << "Successfully added 'MoveSystem': " << std::boolalpha << System::TryAddSystem<MoveSystem>() << std::endl;
        // std::cout << "Successfully removed 'MoveSystem': " << std::boolalpha << System::TryRemoveSystem<MoveSystem>() << std::endl;
        // std::cout << "Successfully removed 'MoveSystem': " << std::boolalpha << System::TryRemoveSystem<MoveSystem>() << std::endl;
        // std::cout << "Successfully added 'MoveSystem': " << std::boolalpha << System::TryAddSystem<MoveSystem>() << std::endl;

        // auto& registry = Registry::Get();
        // auto view = registry.view<SystemStorage>();
        // std::cout << "Systems registered: " << std::distance(view.begin(), view.end()) << std::endl;
        // view.each([](auto entity, auto& storage){
        //     std::cout << "Calling Update()" << std::endl;
        //     storage.system->Update(1.0f);
        // });

        std::cout << "Parent pos: " << parentTransform.GetPos() << std::endl;

        Entity::RequestDestroy(parent);
        mainScene->ProcessCleanup();
        // Entity::ProcessDestructionQueue();

        if (child) std::cout << child.GetName() << " is still alive!" << std::endl;
        else std::cout << "Entity is no longer alive." << std::endl;

        return EXIT_SUCCESS;
    }
    catch (std::exception ex)
    {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown exception thrown!" << std::endl;
    }
    
    return EXIT_FAILURE;
}
