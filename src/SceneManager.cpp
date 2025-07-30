/// @file    SceneManager.cpp
/// @author  Matthew Green
/// @date    2025-07-28 12:20:09
/// 
/// @section LICENSE
/// 
/// Copyright (c) 2025 Matthew Green - All rights reserved
/// Unauthorized copying of this file, via any medium is strictly prohibited
/// Proprietary and confidential

#include "velecs/ecs/SceneManager.hpp"

#include "velecs/ecs/Scene.hpp"

#include <velecs/common/Exceptions.hpp>

namespace velecs::ecs {

// Public Fields

// Constructors and Destructors

// Public Methods

bool SceneManager::TryTransitionScene(const Uuid& uuid)
{
    Scene* scenePtr{nullptr};
    if (!_scenes.TryGetRef(uuid, scenePtr)) return false;
    else return TryTransitionScene(scenePtr);
}

bool SceneManager::TryTransitionScene(const std::string& name)
{
    Scene* scene{nullptr};
    if (!_scenes.TryGetRef(name, scene)) return false;
    else return TryTransitionScene(scene);
}

bool SceneManager::TryReloadCurrentScene()
{
    return TryTransitionScene(_currentScene);
}

bool SceneManager::TryProcess(void* context)
{
    auto scene = GetCurrentScene();
    if (scene == nullptr) return false;

    scene->Process(context);
    return true;
}

bool SceneManager::TryProcessPhysics(void* context)
{
    auto scene = GetCurrentScene();
    if (scene == nullptr) return false;

    scene->ProcessPhysics(context);
    return true;
}

bool SceneManager::TryProcessGUI(void* context)
{
    auto scene = GetCurrentScene();
    if (scene == nullptr) return false;

    scene->ProcessGUI(context);
    return true;
}

bool SceneManager::TryProcessEntityCleanup()
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

bool SceneManager::TryTransitionScene(Scene* const scene)
{
    if (scene == nullptr) return false;

    if (_currentScene != nullptr)
    {
        _currentScene->Cleanup();
    }

    _currentScene = scene;
    _currentScene->Init();

    return true;
}

} // namespace velecs::ecs
