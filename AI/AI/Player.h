#pragma once
#include "DxLib.h"
#include "GameTypes.h"
#include <vector>

// プレイヤーの各種状態と入出力を管理するクラス
// 仕様制約: DxLibの3D座標系（右手系/左手系）に依存したベクトル計算を行う
class Player {
private:
    VECTOR pos{};
    float speed{};
    float radius{};
    int hp{};
    float barrierGauge;
    float maxBarrierGauge;
    PlayerForm currentForm{};
    int muzzleFlashTimer{};
    int shieldBuffTimer{}; 
    
    // レーザー兵器関連状態
    int laserWeaponTimer{};
    float laserHeat;
    int overheatTimer{};
    bool isFiringLaser{};

    // メモリ再確保を防ぐため、事前に十分なcapacityを確保すること
    std::vector<Bullet> bullets;

public:
    Player();
    
    // 入力: なし, 出力: なし, 副作用: プレイヤーの初期状態リセットと座標の初期化
    void Initialize();
    
    // 入力: なし, 出力: なし, 副作用: 入力に基づく座標更新、射撃処理、バフのタイムアウト処理
    void Update();
    
    // 入力: なし, 出力: なし, 副作用: DxLibによる3Dモデル・エフェクトの描画
    void Draw();
    
    VECTOR GetPos() const { return pos; }
    float GetRadius() const { return radius; }
    int GetHP() const { return hp; }
    
    // 入力: ダメージ量, 出力: なし, 副作用: HP減少。シールド中は無効化。
    void Damage(int amount) {
        // シールドバフが有効な場合はダメージを無効化（ゲームバランス設計のため）
        if (amount > 0 && shieldBuffTimer > 0) return; 
        hp -= amount; 
        // 回復アイテム取得時などを想定した上限突破防止
        if (hp > 100) hp = 100; 
    }
    
    void AddShield(int timer) { shieldBuffTimer = timer; }
    int GetShieldTimer() const { return shieldBuffTimer; }
    
    float GetBarrierGauge() const { return barrierGauge; }
    float GetMaxBarrierGauge() const { return maxBarrierGauge; }
    
    // 入力: 消費量, 出力: なし, 副作用: バリアゲージの減少
    void ConsumeBarrier(float amount) { 
        // シールドバフが有効な場合はゲージ消費も無効化
        if (shieldBuffTimer > 0) return; 
        barrierGauge -= amount; 
        if(barrierGauge < 0) barrierGauge = 0; 
    }
    
    // バフ選択時などに強制的に攻撃形態へ戻すため提供
    void ForceAttackForm() { currentForm = PlayerForm::ATTACK; }

    void AddLaserWeapon(int timer) { laserWeaponTimer = timer; laserHeat = 0.0f; overheatTimer = 0; }
    bool HasLaserWeapon() const { return laserWeaponTimer > 0; }
    bool IsFiringLaser() const { return isFiringLaser; }
    float GetLaserHeat() const { return laserHeat; }
    bool IsOverheated() const { return overheatTimer > 0; }
    
    PlayerForm GetForm() const { return currentForm; }
    std::vector<Bullet>& GetBullets() { return bullets; }
};
