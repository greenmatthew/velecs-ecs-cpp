#include <iostream>

#include "velecs/ecs/Common.hpp"
#include "velecs/ecs/SceneManager.hpp"
using namespace velecs::ecs;

#include <velecs/math/Vec3.hpp>
using namespace velecs::math;

#include <stdexcept>

class ExampleTag : public Tag {};

class ExampleComponent : public Component {
    void* _{nullptr}; // All components must store data, otherwise EnTT will treat it like a Tag
};

class ExampleSystem : public System {};

class Velocity : public Component {
public:
    Vec3 vel{Vec3::ZERO};
};

struct SystemContext {
    Scene* scene;
    float deltaTime;
};

class Move : public System {
public:
    void ProcessPhysics(void* context) override
    {
        auto* ctx = Context<SystemContext>(context);
        ctx->scene->Query<Transform, Velocity>([&ctx](auto entity, auto& transform, auto& velocity){
            Vec3 pos = transform.GetPos();
            transform.SetPos(pos + (ctx->deltaTime * velocity.vel));
            Vec3 newPos = transform.GetPos();
            std::cout << entity->GetName() << "'s position: " << pos << " -> " << transform.GetPos() << std::endl;
        });
    }
};

class MainScene : public Scene {
public:
    MainScene(World* const world, const std::string& name, ConstructorKey key)
        : Scene(world, name, key) {}
    
    MainScene(World* const world, const std::string& name, size_t systemCapacity, ConstructorKey key)
        : Scene(world, name, systemCapacity, key) {}

    void OnEnter(void* context)
    {
        Entity* parent = Entity::Create(this)
            .WithName("Parent Entity")
            .WithPos(Vec3::ZERO)
            ;
        Transform& parentTransform = parent->GetTransform();
        std::cout << "Transform: " << parent->GetName() << std::endl;
        std::cout << "\tPosition: " << parentTransform.GetPos() << std::endl;
        std::cout << "\tScale: " << parentTransform.GetScale() << std::endl;
        std::cout << "\tRotation: " << parentTransform.GetEulerAnglesDeg() << std::endl;
        std::cout << "\tModel Matrix:\n" << parentTransform.GetModelMatrix() << std::endl;
        std::cout << "\tWorld Matrix:\n" << parentTransform.GetWorldMatrix() << std::endl;

        Entity* child = Entity::Create(this)
            .WithName("Child Entity")
            .WithParent(parent)
            .WithPos(Vec3::BACKWARD * 10)
            ;
        Transform& childTransform = child->GetTransform();
        std::cout << "Transform: " << child->GetName() << std::endl;
        std::cout << "\tParent:" << (childTransform.GetParent() ? childTransform.GetParent()->GetName() : "(n/a)") << '\n' << std::endl;
        std::cout << "\tPosition: " << childTransform.GetPos() << std::endl;
        std::cout << "\tScale: " << childTransform.GetScale() << std::endl;
        std::cout << "\tRotation: " << childTransform.GetEulerAnglesDeg() << std::endl;
        std::cout << "\tModel Matrix:\n" << childTransform.GetModelMatrix() << std::endl;
        std::cout << "\tWorld Matrix:\n" << childTransform.GetWorldMatrix() << std::endl;

        // Tag system tests
        assert( parent->TryAddTag<ExampleTag>()    && "Should be able to add a tag if entity doesn't already have it");
        assert(!parent->TryAddTag<ExampleTag>()    && "Cannot add the same tag more than once");
        assert(!parent->TryAddTag<ExampleTag>()    && "Cannot add the same tag more than once (second attempt)");
        assert( parent->HasTag<ExampleTag>()       && "Entity should have the tag after adding it");
        assert( parent->TryRemoveTag<ExampleTag>() && "Should be able to remove a tag that exists");
        assert(!parent->TryRemoveTag<ExampleTag>() && "Cannot remove a tag that has already been removed");
        assert(!parent->HasTag<ExampleTag>()       && "Entity should not have the tag after removing it");

        // Component system tests
        const ExampleComponent* _{nullptr};
        assert( parent->TryAddComponent<ExampleComponent>(_)    && "Should be able to add a component if entity doesn't already have it");
        assert(!parent->TryAddComponent<ExampleComponent>(_)    && "Cannot add the same component more than once");
        assert(!parent->TryAddComponent<ExampleComponent>(_)    && "Cannot add the same component more than once (second attempt)");
        assert( parent->HasComponent<ExampleComponent>()        && "Entity should have the component after adding it");
        assert( parent->TryRemoveComponent<ExampleComponent>()  && "Should be able to remove a component that exists");
        assert(!parent->TryRemoveComponent<ExampleComponent>()  && "Cannot remove a component that has already been removed");
        assert(!parent->HasComponent<ExampleComponent>()        && "Entity should not have the component after removing it");

        // System systems tests
        assert( TryAddSystem<ExampleSystem>()    && "Should be able to add a system if scene doesn't already have it");
        assert(!TryAddSystem<ExampleSystem>()    && "Cannot add the same system more than once");
        assert(!TryAddSystem<ExampleSystem>()    && "Cannot add the same system more than once (second attempt)");
        assert( HasSystem<ExampleSystem>()       && "Scene should have the system after adding it");
        assert( TryRemoveSystem<ExampleSystem>() && "Should be able to remove a system that exists");
        assert(!TryRemoveSystem<ExampleSystem>() && "Cannot remove a system that has already been removed");
        assert(!TryRemoveSystem<ExampleSystem>() && "Scene should not have the system after removing it");

        Velocity* vel{nullptr};
        if (parent->TryAddComponent<Velocity>(vel))
        {
            vel->vel = 10.0f * Vec3::RIGHT;
        }

        assert(TryAddSystem<Move>() && "Unable to add Move system.");
    }
};

class TestScene : public Scene {
public:
    TestScene(World* const world, const std::string& name, ConstructorKey key)
        : Scene(world, name, key) {}

    TestScene(World* const world, const std::string& name, size_t systemCapacity, ConstructorKey key)
        : Scene(world, name, systemCapacity, key) {}

    void OnEnter(void* context)
    {
        // Create test hierarchy
        Entity* root1 = Entity::Create(this)
            .WithName("1")
            ;
        
        Entity* child2 = Entity::Create(this)
            .WithName("2")
            .WithParent(root1)
            ;
        
        Entity* child3 = Entity::Create(this)
            .WithName("3")
            .WithParent(root1)
            ;
        
        Entity* grandchild4 = Entity::Create(this)
            .WithName("4")
            .WithParent(child2)
            ;
        
        Entity* grandchild5 = Entity::Create(this)
            .WithName("5")
            .WithParent(child2)
            ;

        Entity* grandchild6 = Entity::Create(this)
            .WithName("6")
            .WithParent(child3)
            ;
        
        Entity* grandchild7 = Entity::Create(this)
            .WithName("7")
            .WithParent(child3)
            ;
        
        Transform& rootTransform = root1->GetTransform();

        std::cout << "Children " << "(count: " << rootTransform.GetChildCount() << "):" << std::endl;
        for (auto entity : rootTransform.GetChildren())
        {
            std::cout << "  " << entity->GetName();
        }
        std::cout << std::endl;
        
        std::cout << "Pre-order:" << std::endl;
        for (auto [entity, transform] : rootTransform.Traverse<TraversalOrder::PreOrder>())
        {
            std::cout << "  " << entity->GetName();
        }

        std::cout << std::endl;

        std::cout << "Post-order:" << std::endl;
        for (auto [entity, transform] : rootTransform.Traverse<TraversalOrder::PostOrder>())
        {
            std::cout << "  " << entity->GetName();
        }
        std::cout << std::endl;

        root1->MarkForDestruction();
    }
};

int main()
{
    try
    {
        auto worldStorage = std::make_unique<World>();
        auto world = worldStorage.get();
        auto mainScene = Scene::Create<MainScene>(world, "Main Scene");
        std::cout << mainScene->GetName() << std::endl;
        auto testScene = Scene::Create<TestScene>(world, "Test Scene");
        std::cout << testScene->GetName() << std::endl;
        Object* obj = Object::Create<Object>(world, "Test Object");
        std::cout << obj->GetName() << std::endl;

        if (world->scenes->TryRequestSceneTransition(mainScene->GetUuid()))
            std::cout << "Successfully request scene transition to: " << mainScene->GetName() << std::endl;

        for (size_t i{0}; i < 5UL; ++i)
        {
            SystemContext systemContext{};
            void* context = static_cast<void*>(&systemContext);
            systemContext.deltaTime = 1.0f;
            systemContext.scene = world->scenes->GetCurrentScene();

            if (world->scenes->Internal_TryTransitionIfRequested(nullptr))
            {
                std::cout << "Transitioned scene to: " << world->scenes->GetCurrentScene()->GetName() << std::endl;
                systemContext.scene = world->scenes->GetCurrentScene();
            }

            world->scenes->Internal_TryProcess(context);
            world->scenes->Internal_TryProcessPhysics(context);
            world->scenes->Internal_TryProcessEntityCleanup();
        }

        if (world->scenes->TryRequestSceneTransition(testScene->GetUuid()))
            std::cout << "Successfully request scene transition to: " << testScene->GetName() << std::endl;
        
        for (size_t i{0}; i < 5UL; ++i)
        {
            SystemContext systemContext{};
            void* context = static_cast<void*>(&systemContext);
            systemContext.deltaTime = 1.0f;
            systemContext.scene = world->scenes->GetCurrentScene();

            if (world->scenes->Internal_TryTransitionIfRequested(nullptr))
            {
                std::cout << "Transitioned scene to: " << world->scenes->GetCurrentScene()->GetName() << std::endl;
                systemContext.scene = world->scenes->GetCurrentScene();
            }

            world->scenes->Internal_TryProcess(context);
            world->scenes->Internal_TryProcessPhysics(context);
            world->scenes->Internal_TryProcessEntityCleanup();
        }
        
        // const auto sceneManager = std::make_unique<SceneManager>();
        // sceneManager->RegisterScene<MainScene>("Main Scene");
        // sceneManager->RegisterScene<TestScene>("Test Scene");

        // SystemContext systemContext{};
        // void* context = static_cast<void*>(&systemContext);
        
        // sceneManager->TryRequestSceneTransition("Main Scene");
        // sceneManager->Internal_TryTransitionIfRequested(nullptr);
        // systemContext.deltaTime = 1.0f;
        // systemContext.scene = sceneManager->GetCurrentScene();
        // for (size_t i{0}; i < 5; ++i)
        // {
        //     sceneManager->Internal_TryProcess(context);
        //     sceneManager->Internal_TryProcessPhysics(context);
        //     sceneManager->Internal_TryProcessEntityCleanup();
        // }
        // sceneManager->TryRequestSceneTransition("Test Scene");
        // sceneManager->Internal_TryTransitionIfRequested(nullptr);
        // for (size_t i{0}; i < 5; ++i)
        // {
        //     sceneManager->Internal_TryProcess(context);
        //     sceneManager->Internal_TryProcessPhysics(context);
        //     sceneManager->Internal_TryProcessEntityCleanup();
        // }
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
