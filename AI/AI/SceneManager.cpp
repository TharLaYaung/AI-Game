#include "SceneManager.h"
#include "SceneManager.h"
#include "OpeningScene.h"
#include "TitleScene.h"
#include "CharacterSelectScene.h"
#include "MainScene.h"
#include "ResultScene.h"
#include "GameOverScene.h"
#include "RankingScene.h"
#include "SettingsScene.h"
#include "LoadingScene.h"
#include "TutorialScene.h"
#include "GameTypes.h"
#include <math.h>

SceneManager::SceneManager() : currentScene(nullptr), currentSceneType(SceneType::OPENING) {}
SceneManager::~SceneManager() {}

void SceneManager::Initialize() {
    
    currentSceneType = SceneType::OPENING;
    currentScene = new OpeningScene();
    currentScene->Initialize();
}

void SceneManager::Update() {
    if (currentScene != nullptr) {
        // 現在のシーンに処理を委譲し、戻り値から次フレームの遷移先を決定する（ステートパターンの適用）
        SceneType nextScene = currentScene->Update();
        
        if (nextScene != currentSceneType) {
            ChangeScene(nextScene);
        }
    }
}

void SceneManager::Draw() {
    if (currentScene != nullptr) {
        currentScene->Draw();
    }

    if (currentSceneType != SceneType::MAIN && currentSceneType != SceneType::LOADING) {
        
        int mouseX, mouseY;
        GetMousePoint(&mouseX, &mouseY);
    
    int time = GetNowCount();
    float angle1 = time * 0.002f;
    float angle2 = -time * 0.003f;
    float scale = 1.0f + 0.1f * sinf(time * 0.005f);

    SetDrawBlendMode(DX_BLENDMODE_ADD, 200);
    
    
    DrawCircle(mouseX, mouseY, 2, GetColor(255, 255, 255), TRUE);
    
    
    int radius = (int)(15 * scale);
    for (int i = 0; i < 3; i++) {
        float a = angle1 + i * (3.14159f * 2.0f / 3.0f);
        int px1 = mouseX + (int)(cosf(a) * radius);
        int py1 = mouseY + (int)(sinf(a) * radius);
        float a_next = angle1 + (i + 1) * (3.14159f * 2.0f / 3.0f);
        int px2 = mouseX + (int)(cosf(a_next) * radius);
        int py2 = mouseY + (int)(sinf(a_next) * radius);
        DrawLine(px1, py1, px2, py2, GetColor(0, 255, 255), 2);
    }
    
        
        int innerRadius = (int)(8 * scale);
        for (int i = 0; i < 4; i++) {
            float a = angle2 + i * (3.14159f * 2.0f / 4.0f);
            int px = mouseX + (int)(cosf(a) * innerRadius);
            int py = mouseY + (int)(sinf(a) * innerRadius);
            DrawLine(mouseX, mouseY, px, py, GetColor(0, 255, 100), 1);
        }

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
    // メモリリーク防止のため、必ず旧シーンのリソースを解放してから新シーンを確保する
    if (currentScene != nullptr) {
        currentScene->Finalize();
        delete currentScene;
    }

    // ポリモーフィズムを利用したシーンインスタンスの動的生成
    switch (nextScene) {
        case SceneType::OPENING:          currentScene = new OpeningScene();          break;
        case SceneType::TITLE:            currentScene = new TitleScene();            break;
        case SceneType::CHARACTER_SELECT: currentScene = new CharacterSelectScene();  break;
        case SceneType::MAIN:             currentScene = new MainScene();             break;
        case SceneType::RESULT:           currentScene = new ResultScene();           break;
        case SceneType::GAMEOVER:         currentScene = new GameOverScene();         break;
        case SceneType::RANKING:          currentScene = new RankingScene();          break;
        case SceneType::SETTINGS:         currentScene = new SettingsScene();         break;
        case SceneType::LOADING:          currentScene = new LoadingScene();          break;
        case SceneType::TUTORIAL:         currentScene = new TutorialScene();         break;
    }

    currentSceneType = nextScene;
    currentScene->Initialize();
}
