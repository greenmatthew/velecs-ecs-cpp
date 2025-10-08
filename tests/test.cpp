#include <velecs/ecs/Common.hpp>
#include <velecs/ecs/SceneManager.hpp>
using namespace velecs::ecs;

#include <velecs/math/Vec3.hpp>
using namespace velecs::math;

#include <gtest/gtest.h>

// Test fixtures and helper classes
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
        ctx->scene->Query<Transform, Velocity>([&ctx](auto, auto& transform, auto& velocity){
            Vec3 pos = transform.GetPos();
            transform.SetPos(pos + (ctx->deltaTime * velocity.vel));
        });
    }
};

class MainScene : public Scene {
public:
    MainScene(World* const world, const std::string& name, size_t systemCapacity)
        : Scene(world, name, systemCapacity) {}

    void OnEnter(void*) override
    {
        Entity* parent = Entity::Create(this)
            .WithName("Parent Entity")
            .WithPos(Vec3::ZERO);

        // Entity::Create(this)
        //     .WithName("Child Entity")
        //     .WithParent(parent)
        //     .WithPos(Vec3::BACKWARD * 10);

        // Velocity* vel{nullptr};
        // if (parent->TryAddComponent<Velocity>(vel))
        // {
        //     vel->vel = 10.0f * Vec3::RIGHT;
        // }

        // TryAddSystem<Move>();
    }
};

class TestScene : public Scene {
public:
    TestScene(World* const world, const std::string& name, size_t systemCapacity)
        : Scene(world, name, systemCapacity) {}

    void OnEnter(void*) override
    {
        // Entity* root1 = Entity::Create(this)
        //     .WithName("1");
        
        // Entity* child2 = Entity::Create(this)
        //     .WithName("2")
        //     .WithParent(root1);
        
        // Entity* child3 = Entity::Create(this)
        //     .WithName("3")
        //     .WithParent(root1);
        
        // Entity::Create(this)
        //     .WithName("4")
        //     .WithParent(child2);
        
        // Entity::Create(this)
        //     .WithName("5")
        //     .WithParent(child2);

        // Entity::Create(this)
        //     .WithName("6")
        //     .WithParent(child3);
        
        // Entity::Create(this)
        //     .WithName("7")
        //     .WithParent(child3);

        // root1->MarkForDestruction();
    }
};

// Test fixture for ECS tests
class ECSTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        _world = std::make_unique<World>();
    }

    void TearDown() override
    {
        _world.reset();
    }

    inline World* GetWorld() const { return _world.get(); }

private:
    std::unique_ptr<World> _world;
};

// Scene tests
TEST_F(ECSTest, SceneCreation)
{
    auto world = GetWorld();

    auto mainScene = Scene::Create<MainScene>(world, "Main Scene");
    auto testScene = Scene::Create<TestScene>(world, "Test Scene");
    
    EXPECT_EQ(mainScene->GetName(), "Main Scene");
    EXPECT_EQ(testScene->GetName(), "Test Scene");
}

TEST_F(ECSTest, SceneTransition1)
{
    auto world = GetWorld();
    auto sceneManager = world->scenes.get();
    auto mainScene = Scene::Create<MainScene>(world, "Main Scene");

    EXPECT_FALSE(sceneManager->GetCurrentScene());

    EXPECT_TRUE(sceneManager->TryRequestSceneTransition("Main Scene"));
    EXPECT_TRUE(sceneManager->Internal_TryTransitionIfRequested(nullptr));

    EXPECT_TRUE(sceneManager->GetCurrentScene());

    auto scene = sceneManager->GetCurrentScene();

    EXPECT_EQ(scene->GetName(), mainScene->GetName());
}

TEST_F(ECSTest, SceneTransition2)
{
    auto world = GetWorld();
    auto sceneManager = world->scenes.get();
    auto mainScene = Scene::Create<MainScene>(world, "Main Scene");
    auto testScene = Scene::Create<TestScene>(world, "Test Scene");

    EXPECT_FALSE(sceneManager->GetCurrentScene());

    EXPECT_TRUE(sceneManager->TryRequestSceneTransition("Main Scene"));
    EXPECT_TRUE(sceneManager->Internal_TryTransitionIfRequested(nullptr));

    EXPECT_TRUE(sceneManager->GetCurrentScene());

    auto scene = sceneManager->GetCurrentScene();

    EXPECT_EQ(scene->GetName(), mainScene->GetName());

    EXPECT_TRUE(sceneManager->TryRequestSceneTransition("Test Scene"));
    EXPECT_TRUE(sceneManager->Internal_TryTransitionIfRequested(nullptr));

    EXPECT_TRUE(sceneManager->GetCurrentScene());

    scene = sceneManager->GetCurrentScene();

    EXPECT_EQ(scene->GetName(), testScene->GetName());
}

// Transform tests
// TEST_F(ECSTest, TransformBasics)
// {
//     auto world = GetWorld();
//     auto scene = world->scenes->GetCurrentScene();
    
//     Entity* parent = Entity::Create(scene)
//         .WithName("Parent Entity")
//         .WithPos(Vec3::ZERO);
    
//     Transform& parentTransform = parent->GetTransform();
    
//     EXPECT_EQ(parentTransform.GetPos(), Vec3::ZERO);
//     EXPECT_EQ(parentTransform.GetScale(), Vec3::ONE);
// }

// TEST_F(ECSTest, TransformHierarchy)
// {
//     auto world = GetWorld();
//     auto scene = world->scenes->GetCurrentScene();
    
//     Entity* parent = Entity::Create(scene)
//         .WithName("Parent Entity")
//         .WithPos(Vec3::ZERO);
    
//     Entity* child = Entity::Create(scene)
//         .WithName("Child Entity")
//         .WithParent(parent)
//         .WithPos(Vec3::BACKWARD * 10);
    
//     Transform& childTransform = child->GetTransform();
    
//     EXPECT_NE(childTransform.GetParent(), nullptr);
//     EXPECT_EQ(childTransform.GetParent()->GetName(), "Parent Entity");
// }

// // Tag system tests
// TEST_F(ECSTest, TagAddRemove)
// {
//     auto world = GetWorld();
//     auto scene = world->scenes->GetCurrentScene();

//     Entity* entity = Entity::Create(scene).WithName("Test Entity");
    
//     EXPECT_TRUE(entity->TryAddTag<ExampleTag>()) << "Should be able to add a tag if entity doesn't already have it";
//     EXPECT_FALSE(entity->TryAddTag<ExampleTag>()) << "Cannot add the same tag more than once";
//     EXPECT_FALSE(entity->TryAddTag<ExampleTag>()) << "Cannot add the same tag more than once (second attempt)";
//     EXPECT_TRUE(entity->HasTag<ExampleTag>()) << "Entity should have the tag after adding it";
//     EXPECT_TRUE(entity->TryRemoveTag<ExampleTag>()) << "Should be able to remove a tag that exists";
//     EXPECT_FALSE(entity->TryRemoveTag<ExampleTag>()) << "Cannot remove a tag that has already been removed";
//     EXPECT_FALSE(entity->HasTag<ExampleTag>()) << "Entity should not have the tag after removing it";
// }

// // Component system tests
// TEST_F(ECSTest, ComponentAddRemove)
// {
//     auto world = GetWorld();
//     auto scene = world->scenes->GetCurrentScene();

//     Entity* entity = Entity::Create(scene).WithName("Test Entity");
    
//     const ExampleComponent* component{nullptr};
//     EXPECT_TRUE(entity->TryAddComponent<ExampleComponent>(component)) << "Should be able to add a component if entity doesn't already have it";
//     EXPECT_FALSE(entity->TryAddComponent<ExampleComponent>(component)) << "Cannot add the same component more than once";
//     EXPECT_FALSE(entity->TryAddComponent<ExampleComponent>(component)) << "Cannot add the same component more than once (second attempt)";
//     EXPECT_TRUE(entity->HasComponent<ExampleComponent>()) << "Entity should have the component after adding it";
//     EXPECT_TRUE(entity->TryRemoveComponent<ExampleComponent>()) << "Should be able to remove a component that exists";
//     EXPECT_FALSE(entity->TryRemoveComponent<ExampleComponent>()) << "Cannot remove a component that has already been removed";
//     EXPECT_FALSE(entity->HasComponent<ExampleComponent>()) << "Entity should not have the component after removing it";
// }

// // System tests
// TEST_F(ECSTest, SystemAddRemove)
// {
//     auto world = GetWorld();
//     auto scene = world->scenes->GetCurrentScene();
    
//     EXPECT_TRUE(scene->TryAddSystem<ExampleSystem>()) << "Should be able to add a system if scene doesn't already have it";
//     EXPECT_FALSE(scene->TryAddSystem<ExampleSystem>()) << "Cannot add the same system more than once";
//     EXPECT_FALSE(scene->TryAddSystem<ExampleSystem>()) << "Cannot add the same system more than once (second attempt)";
//     EXPECT_TRUE(scene->HasSystem<ExampleSystem>()) << "Scene should have the system after adding it";
//     EXPECT_TRUE(scene->TryRemoveSystem<ExampleSystem>()) << "Should be able to remove a system that exists";
//     EXPECT_FALSE(scene->TryRemoveSystem<ExampleSystem>()) << "Cannot remove a system that has already been removed";
//     EXPECT_FALSE(scene->HasSystem<ExampleSystem>()) << "Scene should not have the system after removing it";
// }


// // Object tests
// TEST_F(ECSTest, ObjectCreation)
// {
//     auto world = GetWorld();

//     Object* obj = Object::Create<Object>(world, "Test Object");
//     EXPECT_EQ(obj->GetName(), "Test Object");
// }

// Physics system tests
// TEST_F(ECSTest, PhysicsSystem)
// {
//     auto world = GetWorld();
//     auto scene = world->scenes->GetCurrentScene();

//     world->scenes->TryRequestSceneTransition("Main Scene");
//     world->scenes->Internal_TryTransitionIfRequested(nullptr);
    
//     Entity* entity = Entity::Create(mainScene).WithName("Moving Entity");
//     Velocity* vel{nullptr};
//     ASSERT_TRUE(entity->TryAddComponent<Velocity>(vel));
//     vel->vel = Vec3::RIGHT * 10.0f;
    
//     mainScene->TryAddSystem<Move>();
    
//     Vec3 initialPos = entity->GetTransform().GetPos();
    
//     SystemContext systemContext{};
//     systemContext.deltaTime = 1.0f;
//     systemContext.scene = mainScene;
    
//     world->scenes->Internal_TryProcessPhysics(&systemContext);
    
//     Vec3 finalPos = entity->GetTransform().GetPos();
//     Vec3 expectedPos = initialPos + Vec3::RIGHT * 10.0f;
    
//     EXPECT_EQ(finalPos, expectedPos);
// }

// // Hierarchy traversal tests
// TEST_F(ECSTest, HierarchyTraversal)
// {
//     auto scene = Scene::Create<TestScene>(world.get(), "Test Scene");
//     world->scenes->TryRequestSceneTransition("Test Scene");
//     world->scenes->Internal_TryTransitionIfRequested(nullptr);
    
//     Entity* root1 = Entity::Create(scene).WithName("1");
//     Entity* child2 = Entity::Create(scene).WithName("2").WithParent(root1);
//     Entity* child3 = Entity::Create(scene).WithName("3").WithParent(root1);
//     Entity::Create(scene).WithName("4").WithParent(child2);
//     Entity::Create(scene).WithName("5").WithParent(child2);
//     Entity::Create(scene).WithName("6").WithParent(child3);
//     Entity::Create(scene).WithName("7").WithParent(child3);
    
//     Transform& rootTransform = root1->GetTransform();
    
//     EXPECT_EQ(rootTransform.GetChildCount(), 2);
    
//     // Test pre-order traversal
//     std::vector<std::string> preOrderNames;
//     for (auto [entity, transform] : rootTransform.Traverse<TraversalOrder::PreOrder>())
// {
//         preOrderNames.push_back(entity->GetName());
//     }
    
//     EXPECT_EQ(preOrderNames.size(), 7);
//     EXPECT_EQ(preOrderNames[0], "1");
    
//     // Test post-order traversal
//     std::vector<std::string> postOrderNames;
//     for (auto [entity, transform] : rootTransform.Traverse<TraversalOrder::PostOrder>())
// {
//         postOrderNames.push_back(entity->GetName());
//     }
    
//     EXPECT_EQ(postOrderNames.size(), 7);
// }

// // Entity destruction tests
// TEST_F(ECSTest, EntityDestruction)
// {
//     auto scene = Scene::Create<TestScene>(world.get(), "Test Scene");
//     Entity* entity = Entity::Create(scene).WithName("Test Entity");
    
//     entity->MarkForDestruction();
    
//     world->scenes->Internal_TryProcessEntityCleanup();
    
//     // After cleanup, the entity should be destroyed
//     // Note: You may need to add a way to verify this based on your API
// }
