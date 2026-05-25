#pragma once
#include "Scene.h"

class CharacterSelectScene : public BaseScene {
private:
    int timer;
    int mainFontHandle;
    int subFontHandle;
    int difficultyFontHandle;
    int bgImageHandle;
    
public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
