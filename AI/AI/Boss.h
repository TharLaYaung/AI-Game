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
    void ReviveForNextStage() {
        isSecondForm = false;
        hp = GetMaxHP(); // Restore full HP
        bullets.clear(); // Clear existing bullets
        fireBombs.clear(); // Clear existing fire bombs
        laserStrikeTimer = 0;
        laserStrikeWarningTimer = 0;
    }
    int GetMaxHP() const {
        int baseHP = 300;
        int hpPerStage = 200;
        float diffMultiplier = 0.2f;
        
        if (g_Difficulty == GameDifficulty::NORMAL) {
            baseHP = 500;
            hpPerStage = 500;
            diffMultiplier = 0.25f;
        } else if (g_Difficulty == GameDifficulty::HARD) {
            baseHP = 1000;
            hpPerStage = 1000;
            diffMultiplier = 0.35f;
        }

        float multiplier = 1.0f + (g_CurrentStage - 1) * diffMultiplier;
        return (int)((baseHP + hpPerStage * (g_CurrentStage - 1)) * multiplier);
    }
    void Damage(int amount) { hp -= amount; }
    std::vector<Bullet>& GetBullets() { return bullets; }
    std::vector<FireBomb>& GetFireBombs() { return fireBombs; }
};
