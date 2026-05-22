#pragma once

// シーン（画面）の種類を表すリスト
enum class SceneType {
    TITLE,      // タイトル画面
    CHARACTER_SELECT, // キャラクター（バフ）選択画面
    MAIN,       // メインのゲーム画面
    RESULT,     // リザルト（クリア）画面
    GAMEOVER,   // ゲームオーバー画面
    RANKING     // ランキング画面
};

// 全てのシーンの親となるクラス（基底クラス）
class BaseScene {
public:
    virtual ~BaseScene() {}
    
    // 初期化処理（画像の読み込みや座標のセット）
    virtual void Initialize() = 0;
    
    // 更新処理（入力処理や移動計算）。次に遷移すべきシーンの種類を返します。
    virtual SceneType Update() = 0;
    
    // 描画処理（画像や文字の表示）
    virtual void Draw() = 0;
    
    // 終了処理（画像の解放など）
    virtual void Finalize() = 0;
};
