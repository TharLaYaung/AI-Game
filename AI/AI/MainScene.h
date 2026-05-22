#pragma once
#include "Scene.h"
#include "Player.h"
#include "Boss.h"
#include "EffectManager.h"
#include "GameTypes.h"

struct MiniEnemy {
    VECTOR pos;
    float speed;
    int hp;
    bool active;
};

struct Item {
    VECTOR pos;
    BuffType type;
    bool active;
};

class MainScene : public BaseScene {
private:
    Player* player;
    Boss* boss;
    EffectManager* effectManager;
    int clearTimer;
    
    std::vector<MiniEnemy> miniEnemies;
    std::vector<Item> items;
    
    int bgImageHandle;
    int hudFontHandle;
    int smallFontHandle;

public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
