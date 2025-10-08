#include "velecs/ecs/SceneManager.hpp"
#include "velecs/ecs/Scene.hpp"
#include "velecs/ecs/World.hpp"

#include <velecs/common/Exceptions.hpp>

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

// Public Methods

bool SceneManager::TryRequestSceneTransition(const Uuid& uuid)
{
    Scene* scene = _world->TryGet<Scene>(uuid);
    if (!scene) return false;
    return TryRequestSceneTransition(scene);
}

bool SceneManager::TryRequestSceneTransition(const std::string& name)
{
    std::vector<Scene*> scenes = _world->TryGet<Scene>(name);

    if (scenes.empty())
    {
        std::cerr << "[WARNING] No scenes with name '" << name << "' found." << std::endl;
        return false;
    }

    if (scenes.size() >= 2) std::cerr << "[WARNING] Multiple scenes with name '" << name << "' found. Using first scene." << std::endl;

    Scene* scene = scenes.front();
    return TryRequestSceneTransition(scene);
}

bool SceneManager::TryRequestSceneTransition(Scene* const scene)
{
    if (!scene)
    {
        std::cerr << "[WARNING] Scene transition requested to a nullptr scene." << std::endl;
        return false;
    }
        
    if (_targetScene)
        std::cerr <<
            "[WARNING] Scene transition requested but another transition is already pending. Overriding target scene." << std::endl;

    _targetScene = scene;
    return true;
}

bool SceneManager::TryRequestCurrentSceneReload()
{
    if (_currentScene)
    {
        return TryRequestSceneTransition(_currentScene);
    }
    else
    {
        std::cerr << "[WARNING] Scene reload requested but no scene is currently active." << std::endl;
        return false;
    }
}

size_t SceneManager::GetSceneCount() const
{
    return _world->GetCount<Scene>();
}

bool SceneManager::IsEmpty() const
{
    return _world->GetCount<Scene>() == 0;
}

/// #############################################################
/// ### DO NOT CALL - ENGINE-ONLY FUNCTIONS #####################
/// #############################################################

bool SceneManager::Internal_TryTransitionIfRequested(void* context)
{
    // No transition requested
    if (!_targetScene) return false;

    // Cleanup current scene if one exists
    if (_currentScene != nullptr) _currentScene->Cleanup(context);

    // Transition to target scene
    _currentScene = _targetScene;
    _targetScene = nullptr;

    // Initialize the new scene with context
    _currentScene->Init(context);
    
    return true;
}

bool SceneManager::Internal_TryProcess(void* context)
{
    auto scene = GetCurrentScene();
    if (scene == nullptr) return false;

    scene->Process(context);
    return true;
}

bool SceneManager::Internal_TryProcessPhysics(void* context)
{
    auto scene = GetCurrentScene();
    if (scene == nullptr) return false;

    scene->ProcessPhysics(context);
    return true;
}

bool SceneManager::Internal_TryProcessGUI(void* context)
{
    auto scene = GetCurrentScene();
    if (scene == nullptr) return false;

    scene->ProcessGUI(context);
    return true;
}

bool SceneManager::Internal_TryProcessEntityCleanup()
{
    auto scene = GetCurrentScene();
    if (scene == nullptr) return false;

    scene->ProcessEntityCleanup();
    return true;
}

// Protected Fields

// Protected Methods

// Private Fields

// Private Methods

} // namespace velecs::ecs
