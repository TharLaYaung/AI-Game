#pragma once
#include "Scene.h"

// タイトル画面のライフサイクルとUI制御を担うクラス
// 初回起動時の演出（クロール画面）と、メインメニューの遷移を統合して管理する
class TitleScene : public BaseScene {
private:
    int timer{};
    int titleFontHandle{};
    int subFontHandle{};
    int difficultyFontHandle{};
    int bgImageHandle{};
    int menuIndex{};
    
    // UIナビゲーションの暴走を防ぐための入力ディレイ（フレーム単位）
    int inputCooldown{};
    bool showSettings{};
    
    int introState;
    int introTimer{};
    // クロールテキストをテクスチャとして扱うためのオフスクリーン描画用ハンドル
    int crawlScreenHandle{};
public:
    // 入力: なし, 出力: なし, 副作用: リソースロードおよび初期状態（初回起動時はイントロ、それ以外はメニュー）の設定
    void Initialize() override;
    
    // 入力: なし, 出力: 遷移先シーン, 副作用: 入力に基づくメニュー選択およびイントロ状態の更新
    SceneType Update() override;
    
    // 入力: なし, 出力: なし, 副作用: 状態に応じた3Dテキスト演出（クロール）または2DメニューUIの描画
    void Draw() override;
    
    // 入力: なし, 出力: なし, 副作用: 使用した全フォント・グラフィックハンドルの破棄
    void Finalize() override;
};
