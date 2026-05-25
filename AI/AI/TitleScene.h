#pragma once
#include "Scene.h"

class TitleScene : public BaseScene {
private:
    int timer;
    int titleFontHandle;
    int subFontHandle;
    int difficultyFontHandle;
    int bgImageHandle;
    int menuIndex;
    bool showSettings;
    int inputCooldown;
public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
