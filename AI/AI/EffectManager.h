#pragma once
#include "DxLib.h"
#include <vector>

// 描画負荷軽減およびメモリ断片化を防ぐため、単純な構造体による設計
struct Particle {
    VECTOR pos{};
    VECTOR vel;
    int life;
    int maxLife;
    int color;
    float size;
    bool active;
};

// 画面内の全エフェクトを一元管理するマネージャクラス
// オブジェクトプールパターンに準拠し、動的メモリ確保のコストを最小化する設計
class EffectManager {
private:
    std::vector<Particle> particles;

public:
    EffectManager();
    
    // 入力: なし, 出力: なし, 副作用: コンテナの初期化。事前capacity確保による再確保防止
    void Initialize();
    
    // 入力: なし, 出力: なし, 副作用: 有効な全パーティクルの寿命減算および座標更新
    void Update();
    
    // 入力: なし, 出力: なし, 副作用: アルファブレンド（加算合成）を適用した3D描画
    void Draw();
    
    // 入力: 発生座標, ベース色, 発生数, 出力: なし, 副作用: 非アクティブなインスタンスを再利用して爆発を生成
    void AddExplosion(VECTOR pos, int color, int count);
    
    // 入力: 発生座標, 出力: なし, 副作用: 特殊演出用のワープエフェクトを生成
    void AddTeleport(VECTOR pos);
};
