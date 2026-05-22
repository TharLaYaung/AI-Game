#pragma once
#include "Scene.h"

class TitleScene : public BaseScene {
private:
    int timer;
    int titleFontHandle;
    int subFontHandle;
    int difficultyFontHandle;
    int bgImageHandle;
public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
