#pragma once
#include "Scene.h"

class ResultScene : public BaseScene {
private:
    int timer;
    int mainFontHandle;
    int subFontHandle;
    int bgImageHandle;
public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
