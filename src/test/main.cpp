#include <iostream>

#include "velecs/ecs/Common.hpp"
using namespace velecs::ecs;

#include <velecs/math/Vec3.hpp>
using namespace velecs::math;

#include <stdexcept>

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

class MainScene : public Scene
{
public:
    MainScene(const std::string& name)
        : Scene(name) {}

    MainScene(const std::string& name, Scene::ConstructorKey key)
        : Scene(name, key) {}

    void OnEnter()
    {
        Entity parent = CreateEntity()
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

        Entity child = CreateEntity()
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
        ProcessCleanup();
        // Entity::ProcessDestructionQueue();

        if (child) std::cout << child.GetName() << " is still alive!" << std::endl;
        else std::cout << "Entity is no longer alive." << std::endl;
    }
};

class TestScene : public Scene
{
public:
    TestScene(const std::string& name)
        : Scene(name) {}

    TestScene(const std::string& name, Scene::ConstructorKey key)
        : Scene(name, key) {}

    void OnEnter()
    {
        // Create test hierarchy
        Entity parent = CreateEntity()
            .WithName("Parent")
            .WithPos(Vec3::ZERO)
            ;
        
        Entity child1 = CreateEntity()
            .WithName("Child1")
            .WithParent(parent)
            .WithPos(Vec3::RIGHT)
            ;
        
        Entity child2 = CreateEntity()
            .WithName("Child2")
            .WithParent(parent)
            .WithPos(Vec3::LEFT)
            ;
        
        Entity grandchild = CreateEntity()
            .WithName("Grandchild")
            .WithParent(child1)
            .WithPos(Vec3::UP)
            ;
        
        Transform& parentTransform = parent.GetTransform();

        std::cout << "Children " << "(count: " << parentTransform.GetChildCount() << "):" << std::endl;
        for (auto entity : parentTransform.GetChildren())
        {
            std::cout << "  " << entity.GetName() << std::endl;
        }
        
        // std::cout << "Pre-order:" << std::endl;
        // for (auto [entity, transform] : parentTransform.Traverse<TraversalOrder::PreOrder>())
        // {
        //     std::cout << "  " << entity.GetName() << std::endl;
        // }
    }
};

int main()
{
    try
    {
        const auto currentSceneStorage = std::make_unique<TestScene>("Main Scene");
        const auto currentScene = currentSceneStorage.get();
        currentScene->OnEnter();
        currentScene->OnExit();
    }
    catch (std::exception ex)
    {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return EXIT_FAILURE;

    }
    catch (...)
    {
        std::cerr << "Unknown exception thrown!" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
