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
    int timer; 
};

struct Star {
    VECTOR pos;
    float speed;
    int color;
};

enum class MainState {
    PLAYING,
    MID_BUFF_SELECT
};

class MainScene : public BaseScene {
private:
    Player* player;
    Boss* boss;
    EffectManager* effectManager;
    int clearTimer;
    
    MainState currentState;
    int midBuffIndex;
    int midBuffCooldown;
    int buffSelectTimer;
    
    int cardLaserImg;
    int cardBombImg;
    int cardSpeedImg;
    
    std::vector<MiniEnemy> miniEnemies;
    std::vector<Item> items;
    std::vector<Star> stars;
    
    VECTOR camPos;
    bool isPaused;
    
    int hudFontHandle;
    int smallFontHandle;
    
    int realmWarpTimer;
    int realmColorMode;

public:
    void Initialize() override;
    SceneType Update() override;
    void Draw() override;
    void Finalize() override;
};
