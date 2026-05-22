#pragma once
#include "DxLib.h"
#include <vector>

// 弾の構造体
struct Bullet {
    VECTOR pos;      // 位置
    VECTOR dir;      // 進行方向
    float speed;     // スピード
    float radius;    // 当たり判定の半径
    bool active;     // 有効かどうか
    bool isReflected;// ボスに跳ね返された弾かどうか
};

// プレイヤーのフォーム
enum class PlayerForm {
    ATTACK,
    BARRIER
};

enum class GameDifficulty {
    EASY,
    NORMAL,
    HARD
};

extern GameDifficulty g_Difficulty;
