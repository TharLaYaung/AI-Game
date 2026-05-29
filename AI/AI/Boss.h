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
    VECTOR pos{};
    // プレイヤーに退避猶予を与えるための起爆前タイマー
    int warningTimer; 
    // 描画および当たり判定の持続フレーム数
    int activeTimer;  
    float radius{};
    bool active;
};

// ボスの状態・攻撃（弾、レーザー、ボム）を管理するクラス
// 仕様制約: 進行度（ステージ）と難易度（難易度設定）に強く依存したステータス計算を行う
class Boss {
private:
    VECTOR pos{};
    VECTOR targetPos{};
    float radius{};
    int hp{};
    
    BossState state{};
    int stateTimer{};
    int laserStrikeTimer{};
    int laserStrikeWarningTimer{};
    VECTOR laserStrikePos{};
    bool isSecondForm{};
    
    // パフォーマンス低下を防ぐため、オブジェクトプール的な再利用を想定
    std::vector<Bullet> bullets;
    std::vector<FireBomb> fireBombs;

public:
    Boss();
    
    // 入力: なし, 出力: なし, 副作用: ボスの初期座標・HP等の初期化
    void Initialize();
    
    // 入力: プレイヤー座標, 出力: なし, 副作用: 状態遷移、弾の発射、大技のタイマー更新
    void Update(VECTOR playerPos);
    
    // 入力: なし, 出力: なし, 副作用: ボス本体および付随する攻撃の描画
    void Draw();

    VECTOR GetPos() const { return pos; }
    float GetRadius() const { return radius; }
    int GetHP() const { return hp; }
    int GetLaserStrikeTimer() const { return laserStrikeTimer; }
    int GetLaserStrikeWarningTimer() const { return laserStrikeWarningTimer; }
    VECTOR GetLaserStrikePos() const { return laserStrikePos; }
    bool IsSecondForm() const { return isSecondForm; }
    
    // 入力: なし, 出力: なし, 副作用: 次ステージ用のHP再計算と状態リセット
    void ReviveForNextStage() {
        isSecondForm = false;
        hp = GetMaxHP(); 
        bullets.clear(); 
        fireBombs.clear(); 
        laserStrikeTimer = 0;
        laserStrikeWarningTimer = 0;
        pos = VGet(0.0f, 150.0f, 300.0f);
        targetPos = pos;
        state = BossState::IDLE;
        stateTimer = 0;
    }
    
    // ゲーム進行によるインフレに対応するため、ステージ数と難易度に応じたスケーリング式を適用
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

        // floatからintへの暗黙の縮小変換(C4244)を意図的に許容（UI表示等のため整数化必須）
        float multiplier = 1.0f + (g_CurrentStage - 1) * diffMultiplier;
        return (int)((baseHP + hpPerStage * (g_CurrentStage - 1)) * multiplier);
    }
    
    void Damage(int amount) { hp -= amount; }
    std::vector<Bullet>& GetBullets() { return bullets; }
    std::vector<FireBomb>& GetFireBombs() { return fireBombs; }
};
