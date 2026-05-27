#pragma once
#include "Scene.h"
#include <string>

class RankingScene : public BaseScene {
private:
    int timer;
    int titleFontHandle;
    int subFontHandle;
    int inputFontHandle;
    
    bool isInputtingName;
    int keyInputHandle;
    
    int keyWaitTimer;
public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
