#pragma once
#include "Scene.h"

class GameOverScene : public BaseScene {
private:
    int timer;
    int mainFontHandle;
    int subFontHandle;
public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
