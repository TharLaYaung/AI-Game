#pragma once
#include "Scene.h"
#include "GameTypes.h"

class SettingsScene : public BaseScene {
private:
    int fontHandle;
    int smallFontHandle;
    int cursorIndex;
    int keyWaitTimer;
    int timer;
    bool isWaitingForKey;
public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
