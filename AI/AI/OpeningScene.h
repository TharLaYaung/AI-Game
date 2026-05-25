#pragma once
#include "Scene.h"
#include "GameTypes.h"

class OpeningScene : public BaseScene {
private:
    int timer;
    int titleFontHandle;
    int smallFontHandle;
    int cyberFontHandle;
    int bgImageHandle;

public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
