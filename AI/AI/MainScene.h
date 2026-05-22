#pragma once
#include "Scene.h"
#include "Player.h"
#include "Boss.h"
#include "EffectManager.h"

class MainScene : public BaseScene {
private:
    Player* player;
    Boss* boss;
    EffectManager* effectManager;

    int clearTimer;
    
    int bgImageHandle;
    int hudFontHandle;
    int smallFontHandle;

public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
