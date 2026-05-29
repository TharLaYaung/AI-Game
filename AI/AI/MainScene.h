#pragma once
#include "Scene.h"
#include "Player.h"
#include "Boss.h"
#include "EffectManager.h"
#include "GameTypes.h"

struct MiniEnemy {
    VECTOR pos{};
    float speed{};
    int hp{};
    bool active;
};

// 描画および当たり判定の最適化のため、各種アイテムもプール管理を前提とする
struct Item {
    VECTOR pos{};
    BuffType type;
    bool active;
    // 画面外等での残留を防ぐための寿命タイマー
    int timer{}; 
};

struct Star {
    VECTOR pos{};
    float speed{};
    int color;
};

// ゲームプレイとバフ選択画面の状態管理（ポーズとは独立して処理分岐するため分離）
enum class MainState {
    PLAYING,
    MID_BUFF_SELECT
};

// ゲームプレイのメインループと描画を統合管理するクラス
// 3Dと2D(UI)の混在描画を適切に処理する責務を持つ
class MainScene : public BaseScene {
private:
    Player* player{};
    Boss* boss{};
    EffectManager* effectManager{};
    int clearTimer{};
    
    MainState currentState{};
    int midBuffIndex{};
    int midBuffCooldown{};
    int buffSelectTimer{};
    
    int cardLaserImg{};
    int cardBombImg{};
    int cardSpeedImg{};
    
    std::vector<MiniEnemy> miniEnemies;
    std::vector<Item> items;
    std::vector<Star> stars;
    
    // 疑似的なスクロール表現を行うためのカメラ座標
    VECTOR camPos{};
    bool isPaused{};
    
    int hudFontHandle{};
    int smallFontHandle{};
    
    int gameTimer{};
    
    // ステージクリア時のワープ演出進行度
    int realmWarpTimer{};
    // 背景色（異次元）の表現切替フラグ
    int realmColorMode;

public:
    // 入力: なし, 出力: なし, 副作用: 依存オブジェクトの生成と各種リソース（画像・フォント）のロード
    void Initialize() override;
    
    // 入力: なし, 出力: SceneType, 副作用: 入力処理、衝突判定、状態遷移。他シーンへの遷移指示を返す
    SceneType Update() override;
    
    // 入力: なし, 出力: なし, 副作用: カメラ設定、3D描画、UI（2D）描画の順次実行
    void Draw() override;
    
    // 入力: なし, 出力: なし, 副作用: ロード済みリソースの解放および動的確保メモリの破棄
    void Finalize() override;
};
