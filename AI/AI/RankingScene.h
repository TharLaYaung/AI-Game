#pragma once
#include "Scene.h"

class RankingScene : public BaseScene {
public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
