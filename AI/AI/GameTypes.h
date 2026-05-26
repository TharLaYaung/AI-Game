#pragma once
#include "DxLib.h"
#include <vector>


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
};

extern GameDifficulty g_Difficulty;
extern BuffType g_Buff;
extern int g_BuffLevel;
extern int g_Score;
extern std::vector<RankRecord> g_Ranking;
extern bool g_ExitGame;

#include "Scene.h"
extern SceneType g_TargetScene;

void LoadRanking();
void SaveRanking(const std::string& name, int newScore);
