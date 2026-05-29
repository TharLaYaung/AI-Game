#pragma once
#include "Scene.h"
#include "GameTypes.h"

class CharacterSelectScene : public BaseScene {
private:
    int timer{};
    int mainFontHandle{};
    int subFontHandle{};
    int difficultyFontHandle{};
    int bgImageHandle{};
    BuffType selectedBuff;
    
public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
