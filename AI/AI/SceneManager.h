#pragma once
#include "Scene.h"

class SceneManager {
private:
    BaseScene* currentScene;    // 現在表示しているシーンの本体
    SceneType currentSceneType; // 現在表示しているシーンの種類

    // シーンを切り替える内部関数
    void ChangeScene(SceneType nextScene);

public:
    SceneManager();
    ~SceneManager();

    void Initialize();
    void Update();
    void Draw();
    void Finalize();
};
