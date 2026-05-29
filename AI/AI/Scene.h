#pragma once


#pragma once

// アプリケーション全体のシーン遷移（ステートマシン）を定義する列挙型
enum class SceneType {
    OPENING,    
    TITLE,      
    CHARACTER_SELECT, 
    MAIN,       
    RESULT,     
    GAMEOVER,   
    RANKING,    
    LOADING,
    SETTINGS,
    TUTORIAL
};

// 全シーンの基底クラス。ポリモーフィズムを利用してSceneManagerから一元管理するためのインターフェース
class BaseScene {
public:
    virtual ~BaseScene() {}
    
    // 入力: なし, 出力: なし, 副作用: リソースの確保と初期状態のリセット
    virtual void Initialize() = 0;
    
    // 入力: なし, 出力: 遷移先のSceneType, 副作用: 入力に基づく状態更新
    // 戻り値が自身のSceneTypeと異なる場合、SceneManagerによって破棄・再生成される
    virtual SceneType Update() = 0;
    
    // 入力: なし, 出力: なし, 副作用: VRAMへの描画コマンド発行（バックバッファへの描画）
    virtual void Draw() = 0;
    
    // 入力: なし, 出力: なし, 副作用: 確保したリソース(画像,BGM等)の明示的な解放によるメモリリーク防止
    virtual void Finalize() = 0;
};
