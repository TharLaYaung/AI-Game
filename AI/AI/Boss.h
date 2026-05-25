#pragma once
#include "DxLib.h"
#include "GameTypes.h"
#include <vector>

enum class BossState {
    IDLE,
    ATTACKING,
    MOVING
};

struct FireBomb {
    VECTOR pos;
    int warningTimer; // Time until explosion
    int activeTimer;  // Time explosion lasts
    float radius;
    bool active;
};

class Boss {
private:
    VECTOR pos;
    VECTOR targetPos;
    float radius;
    int hp;
    
    BossState state;
    int stateTimer;
    int laserStrikeTimer;
    int laserStrikeWarningTimer;
    VECTOR laserStrikePos;
    bool isSecondForm;
    bool isFinalForm;
    
    std::vector<Bullet> bullets;
    std::vector<FireBomb> fireBombs;

public:
    Boss();
    void Initialize();
    void Update(VECTOR playerPos);
    void Draw();

    VECTOR GetPos() const { return pos; }
    float GetRadius() const { return radius; }
    int GetHP() const { return hp; }
    int GetLaserStrikeTimer() const { return laserStrikeTimer; }
    int GetLaserStrikeWarningTimer() const { return laserStrikeWarningTimer; }
    VECTOR GetLaserStrikePos() const { return laserStrikePos; }
    bool IsSecondForm() const { return isSecondForm; }
    bool IsFinalForm() const { return isFinalForm; }
    void EnterFinalForm() {
        isFinalForm = true;
        hp = GetMaxHP(); // Restore full HP
        bullets.clear(); // Clear existing bullets
        fireBombs.clear(); // Clear existing fire bombs
        laserStrikeTimer = 0;
        laserStrikeWarningTimer = 0;
    }
    int GetMaxHP() const {
        if (g_Difficulty == GameDifficulty::EASY) return 300;
        if (g_Difficulty == GameDifficulty::NORMAL) return 500;
        return 1000;
    }
    void Damage(int amount) { hp -= amount; }
    std::vector<Bullet>& GetBullets() { return bullets; }
    std::vector<FireBomb>& GetFireBombs() { return fireBombs; }
};
