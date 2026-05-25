#pragma once
#include "Scene.h"

class LoadingScene : public BaseScene {
private:
    int timer;
    int fontHandle;
    int smallFontHandle;
    int bgImageHandle;
    int type; 
public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
