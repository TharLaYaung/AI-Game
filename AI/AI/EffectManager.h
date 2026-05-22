#pragma once
#include "DxLib.h"
#include <vector>

struct Particle {
    VECTOR pos;
    VECTOR vel;
    int life;
    int maxLife;
    int color;
    float size;
    bool active;
};

class EffectManager {
private:
    std::vector<Particle> particles;

public:
    EffectManager();
    void Initialize();
    void Update();
    void Draw();
    
    // エフェクトを発生させる
    void AddExplosion(VECTOR pos, int color, int count);
    void AddTeleport(VECTOR pos);
};
