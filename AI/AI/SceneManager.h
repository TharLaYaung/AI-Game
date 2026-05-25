#pragma once
#include "Scene.h"

class SceneManager {
private:
    BaseScene* currentScene;    
    SceneType currentSceneType; 

    
    void ChangeScene(SceneType nextScene);

public:
    SceneManager();
    ~SceneManager();

    void Initialize();
    void Update();
    void Draw();
    void Finalize();
};
