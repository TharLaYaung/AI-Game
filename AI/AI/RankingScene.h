#pragma once
#include "Scene.h"

class RankingScene : public BaseScene {
private:
    int timer;
    int titleFontHandle;
    int subFontHandle;
    int inputFontHandle;
    
    bool isInputtingName;
    char nameChars[3];
    int charIndex;
    
    int keyWaitTimer;
public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
