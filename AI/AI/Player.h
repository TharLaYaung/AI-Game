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
    float barrierGauge;
    float maxBarrierGauge;
    PlayerForm currentForm;
    int muzzleFlashTimer;
    int shieldBuffTimer; 
    
    int laserWeaponTimer;
    float laserHeat;
    int overheatTimer;
    bool isFiringLaser;

    std::vector<Bullet> bullets;

public:
    Player();
    void Initialize();
    void Update();
    void Draw();
    
    
    VECTOR GetPos() const { return pos; }
    float GetRadius() const { return radius; }
    int GetHP() const { return hp; }
    void Damage(int amount) {
        if (amount > 0 && shieldBuffTimer > 0) return; 
        hp -= amount; 
        if (hp > 100) hp = 100; 
    }
    void AddShield(int timer) { shieldBuffTimer = timer; }
    int GetShieldTimer() const { return shieldBuffTimer; }
    
    float GetBarrierGauge() const { return barrierGauge; }
    float GetMaxBarrierGauge() const { return maxBarrierGauge; }
    void ConsumeBarrier(float amount) { 
        if (shieldBuffTimer > 0) return; 
        barrierGauge -= amount; 
        if(barrierGauge < 0) barrierGauge = 0; 
    }
    void ForceAttackForm() { currentForm = PlayerForm::ATTACK; }

    void AddLaserWeapon(int timer) { laserWeaponTimer = timer; laserHeat = 0.0f; overheatTimer = 0; }
    bool HasLaserWeapon() const { return laserWeaponTimer > 0; }
    bool IsFiringLaser() const { return isFiringLaser; }
    float GetLaserHeat() const { return laserHeat; }
    bool IsOverheated() const { return overheatTimer > 0; }
    
    PlayerForm GetForm() const { return currentForm; }
    std::vector<Bullet>& GetBullets() { return bullets; }
};
