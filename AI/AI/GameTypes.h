#pragma once
#include "DxLib.h"
#include <vector>
#include <map>


struct Bullet {
    VECTOR pos;      
    VECTOR dir;      
    float speed;     
    float radius;    
    bool active;     
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

enum class BuffType {
    NONE,
    LASER,
    BOMB,
    SPEED,
    HEAL,
    SHIELD,
    LASER_WEAPON
};

#include <string>

struct RankRecord {
    std::string name;
    int score;
    int clearTime; // frames
};

extern GameDifficulty g_Difficulty;
extern std::map<BuffType, int> g_Buffs;
extern int g_CurrentStage;
extern int g_Score;
extern int g_ClearTime;
extern int g_BGMVolume;
extern int g_SEVolume;
extern std::vector<RankRecord> g_Ranking;
extern bool g_ExitGame;

// Configurable Key Bindings
extern int g_KeyMoveUp;
extern int g_KeyMoveDown;
extern int g_KeyMoveLeft;
extern int g_KeyMoveRight;
extern int g_KeyAltMoveUp;
extern int g_KeyAltMoveDown;
extern int g_KeyAttack;
extern int g_KeyBarrier;
extern int g_KeyPause;

const wchar_t* GetKeyName(int keyCode);

#include "Scene.h"
extern SceneType g_TargetScene;

void LoadRanking();
void SaveRanking(const std::string& name, int newScore);
void SaveSettings();
