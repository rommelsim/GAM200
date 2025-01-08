#include "pch.h"
#include <SESceneManager.h>

//std::unique_ptr<SceneManager> _instance{ nullptr };
//
//std::unique_ptr<SceneManager>& SceneManager::GetInstance()
//{
//    if (_instance == nullptr)
//    {
//        _instance = std::make_unique<SceneManager>();
//    }
//    return _instance;
//}
//
//void SceneManager::AddScene(Scene* scene) {
//    scenes_.push_back(scene);
//}
//
//void SceneManager::GoToScene(int scene_index) {
//    current_scene_index_ = scene_index;
//    current_scene_ = scenes_[current_scene_index_];
//    current_scene_->Init();
//}
//
//void SceneManager::NextScene() {
//    current_scene_->Clear();
//    current_scene_index_ = (current_scene_index_ + 1) % scenes_.size();
//    current_scene_ = scenes_[current_scene_index_];
//    current_scene_->Init();
//}
//
//void SceneManager::Update(float dt) {
//    current_scene_->Update(dt);
//}
//
//void SceneManager::Render() {
//    current_scene_->Render();
//}
