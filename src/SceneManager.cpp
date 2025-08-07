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

bool SceneManager::TryRequestSceneTransition(const Uuid& uuid)
{
    Scene* scene{nullptr};
    if (_scenes.TryGetRef(uuid, scene)) return TryRequestSceneTransition(scene);
    else return false;
}

bool SceneManager::TryRequestSceneTransition(const std::string& name)
{
    Scene* scene{nullptr};
    if (_scenes.TryGetRef(name, scene)) return TryRequestSceneTransition(scene);
    else return false;
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
