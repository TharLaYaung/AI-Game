#include "SceneManager.h"
#include "TitleScene.h"
#include "CharacterSelectScene.h"
#include "MainScene.h"
#include "ResultScene.h"
#include "GameOverScene.h"
#include "RankingScene.h"

SceneManager::SceneManager() : currentScene(nullptr), currentSceneType(SceneType::TITLE) {}
SceneManager::~SceneManager() {}

void SceneManager::Initialize() {
    // 最初はタイトル画面からスタートする
    currentSceneType = SceneType::TITLE;
    currentScene = new TitleScene();
    currentScene->Initialize();
}

void SceneManager::Update() {
    if (currentScene != nullptr) {
        // 現在のシーンを更新し、次に遷移すべきシーンを受け取る
        SceneType nextScene = currentScene->Update();
        
        // もし「現在のシーン」と「返ってきたシーン」が違っていたら、画面を切り替える
        if (nextScene != currentSceneType) {
            ChangeScene(nextScene);
        }
    }
}

void SceneManager::Draw() {
    if (currentScene != nullptr) {
        currentScene->Draw();
    }
}

void SceneManager::Finalize() {
    if (currentScene != nullptr) {
        currentScene->Finalize();
        delete currentScene;
        currentScene = nullptr;
    }
}

void SceneManager::ChangeScene(SceneType nextScene) {
    // 1. 現在のシーンの終了処理とメモリ解放
    if (currentScene != nullptr) {
        currentScene->Finalize();
        delete currentScene;
    }

    // 2. 指定された新しいシーンを作成
    switch (nextScene) {
        case SceneType::TITLE:            currentScene = new TitleScene();            break;
        case SceneType::CHARACTER_SELECT: currentScene = new CharacterSelectScene();  break;
        case SceneType::MAIN:             currentScene = new MainScene();             break;
        case SceneType::RESULT:           currentScene = new ResultScene();           break;
        case SceneType::GAMEOVER:         currentScene = new GameOverScene();         break;
        case SceneType::RANKING:          currentScene = new RankingScene();          break;
    }

    // 3. 新しいシーンの初期化
    currentSceneType = nextScene;
    currentScene->Initialize();
}
