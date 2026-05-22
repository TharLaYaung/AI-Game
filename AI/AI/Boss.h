#pragma once
#include "DxLib.h"
#include "GameTypes.h"
#include <vector>

enum class BossState {
    IDLE,
    ATTACKING,
    TELEPORTING
};

class Boss {
private:
    VECTOR pos;
    float radius;
    int hp;
    
    BossState state;
    int stateTimer;
    
    std::vector<Bullet> bullets;

public:
    Boss();
    void Initialize();
    void Update(VECTOR playerPos);
    void Draw();

    VECTOR GetPos() const { return pos; }
    float GetRadius() const { return radius; }
    int GetHP() const { return hp; }
    void Damage(int amount) { hp -= amount; }
    std::vector<Bullet>& GetBullets() { return bullets; }
};
