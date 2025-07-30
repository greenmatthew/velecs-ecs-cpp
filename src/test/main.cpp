#include <iostream>

#include "velecs/ecs/Common.hpp"
using namespace velecs::ecs;

#include <velecs/math/Vec3.hpp>
using namespace velecs::math;

#include <stdexcept>

class ExampleTag : public Tag {};

class Velocity : public Component {
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

class MainScene : public Scene {
public:
    MainScene(const std::string& name, ConstructorKey key)
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

        Velocity* vel{nullptr};
        if (parent.TryAddComponent<Velocity>(vel))
        {
            const Velocity* _{nullptr};
            assert(!parent.TryAddComponent<Velocity>(_) && "Cannot add a Component more than once");
            assert(!parent.TryAddComponent<Velocity>(_) && "Cannot add a Component more than once");
            
            vel->vel = Vec3::RIGHT;
        }
        
        // vel = parent.TryAddComponent<Velocity>();
        // vel = parent.TryAddComponent<Velocity>();
        
        
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

        if (parent.TryAddTag<ExampleTag>())
        {
            assert(!parent.TryAddTag<ExampleTag>() && "Cannot add a Tag more than once");
            assert(!parent.TryAddTag<ExampleTag>() && "Cannot add a Tag more than once");
        }
        assert(parent.TryRemoveTag<ExampleTag>() && "Should be able to remove a Tag once it has been added");
        assert(!parent.TryRemoveTag<ExampleTag>() && "Should not be able to remove a Tag once it has already been removed");

        parent.MarkForDestruction();
    }
};

class TestScene : public Scene {
public:
    TestScene(const std::string& name, ConstructorKey key)
        : Scene(name, key) {}

    void OnEnter()
    {
        // Create test hierarchy
        Entity root1 = CreateEntity()
            .WithName("1")
            ;
        
        Entity child2 = CreateEntity()
            .WithName("2")
            .WithParent(root1)
            ;
        
        Entity child3 = CreateEntity()
            .WithName("3")
            .WithParent(root1)
            ;
        
        Entity grandchild4 = CreateEntity()
            .WithName("4")
            .WithParent(child2)
            ;
        
        Entity grandchild5 = CreateEntity()
            .WithName("5")
            .WithParent(child2)
            ;

        Entity grandchild6 = CreateEntity()
            .WithName("6")
            .WithParent(child3)
            ;
        
        Entity grandchild7 = CreateEntity()
            .WithName("7")
            .WithParent(child3)
            ;
        
        Transform& rootTransform = root1.GetTransform();

        std::cout << "Children " << "(count: " << rootTransform.GetChildCount() << "):" << std::endl;
        for (auto entity : rootTransform.GetChildren())
        {
            std::cout << "  " << entity.GetName();
        }
        std::cout << std::endl;
        
        std::cout << "Pre-order:" << std::endl;
        for (auto [entity, transform] : rootTransform.Traverse<TraversalOrder::PreOrder>())
        {
            std::cout << "  " << entity.GetName();
        }

        std::cout << std::endl;

        // std::cout << "In-order:" << std::endl;
        // for (auto [entity, transform] : rootTransform.Traverse<TraversalOrder::InOrder>())
        // {
        //     std::cout << "  " << entity.GetName();
        // }
        // std::cout << std::endl;

        std::cout << "Post-order:" << std::endl;
        for (auto [entity, transform] : rootTransform.Traverse<TraversalOrder::PostOrder>())
        {
            std::cout << "  " << entity.GetName();
        }
        std::cout << std::endl;

        // std::cout << "Level-order:" << std::endl;
        // for (auto [entity, transform] : rootTransform.Traverse<TraversalOrder::LevelOrder>())
        // {
        //     std::cout << "  " << entity.GetName();
        // }
        // std::cout << std::endl;

        // std::cout << "Reverse Level-order:" << std::endl;
        // for (auto [entity, transform] : rootTransform.Traverse<TraversalOrder::ReverseLevelOrder>())
        // {
        //     std::cout << "  " << entity.GetName();
        // }
        // std::cout << std::endl;

        root1.MarkForDestruction();
    }
};

int main()
{
    try
    {
        const auto sceneManager = std::make_unique<SceneManager>();
        sceneManager->RegisterScene<MainScene>("Main Scene");
        sceneManager->RegisterScene<TestScene>("Test Scene");
        sceneManager->TryTransitionScene("Main Scene");
        sceneManager->TryCleanupCurrentScene();
        sceneManager->TryTransitionScene("Test Scene");
        sceneManager->TryCleanupCurrentScene();
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
