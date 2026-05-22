#pragma once
#include "DxLib.h"
#include "GameTypes.h"
#include <vector>

class Player {
private:
    VECTOR pos;
    float speed;
    float radius;
    int hp;
    PlayerForm currentForm;

    std::vector<Bullet> bullets;

public:
    Player();
    void Initialize();
    void Update();
    void Draw();
    
    // ゲッターなど
    VECTOR GetPos() const { return pos; }
    float GetRadius() const { return radius; }
    int GetHP() const { return hp; }
    void Damage(int amount) { hp -= amount; }
    PlayerForm GetForm() const { return currentForm; }
    std::vector<Bullet>& GetBullets() { return bullets; }
};
