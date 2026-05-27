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
    bool showSettings; // Actually we will move to SceneType::SETTINGS, so this might be removed. But keep for now.
    int inputCooldown;
    
    int introState;
    int introTimer;
    int crawlScreenHandle;
public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
