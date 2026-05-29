#pragma once
#include "DxLib.h"
#include <vector>
#include <map>
#include <string>

// オブジェクト間で共有する物理挙動用データ構造
// メモリの局所性を高めキャッシュヒット率を上げるため、クラスではなくPOD(Plain Old Data)として定義
struct Bullet {
    VECTOR pos{};      
    VECTOR dir;      
    float speed{};     
    float radius{};    
    bool active;     
    // 反射弾としての振る舞い（ボスへのダメージ化）を区別するためのフラグ
    bool isReflected;
};

enum class PlayerForm {
    ATTACK,
    BARRIER
};

enum class GameDifficulty {
    EASY,
    NORMAL,
    HARD
};

// バフ管理用列挙型。配列のインデックスやMapのキーとして利用し、ステータス計算式へ直結させる
enum class BuffType {
    NONE,
    LASER,
    BOMB,
    SPEED,
    HEAL,
    SHIELD,
    LASER_WEAPON
};

// ランキング表示用のデータレコード。ファイル永続化の対象となる
struct RankRecord {
    std::string name;
    int score{};
    int clearTime; // frames
};

// --- グローバル変数 ---
// シーン間でのデータ引き継ぎ（スコア、進行度、設定等）を簡略化するためグローバル配置
extern GameDifficulty g_Difficulty;
extern std::map<BuffType, int> g_Buffs;
extern int g_CurrentStage;
extern int g_Score;
extern int g_ClearTime;
extern int g_BGMVolume;
extern int g_SEVolume;
extern std::vector<RankRecord> g_Ranking;
extern bool g_ExitGame;
// 初回起動判定。タイトル前のチュートリアル表示制御に使用
extern bool g_IsFirstRun;

// --- キーコンフィグ ---
// プレイヤーが任意に変更可能な入力設定を保持
extern int g_KeyMoveUp;
extern int g_KeyMoveDown;
extern int g_KeyMoveLeft;
extern int g_KeyMoveRight;
extern int g_KeyAltMoveUp;
extern int g_KeyAltMoveDown;
extern int g_KeyAttack;
extern int g_KeyBarrier;
extern int g_KeyPause;

// 入力: DxLibのキーコード, 出力: 文字列, 副作用: なし
const wchar_t* GetKeyName(int keyCode);

#include "Scene.h"
// ロード画面経由の非同期遷移を制御するための目標シーンバッファ
extern SceneType g_TargetScene;

// ランキングと設定の永続化I/O（入力: なし, 出力: なし, 副作用: ローカルファイルシステムへのアクセス）
void LoadRanking();
void SaveRanking(const std::string& name, int newScore);
void SaveSettings();
