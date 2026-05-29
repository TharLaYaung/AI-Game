#pragma once
#include "Scene.h"
#include "GameTypes.h"

class TutorialScene : public BaseScene {
private:
    int fontHandle{};
    int smallFontHandle{};
    int timer{};
public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
