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

extern GameDifficulty g_Difficulty;
extern BuffType g_Buff;
extern int g_Score;
extern std::vector<int> g_Ranking;
extern bool g_ExitGame;

#include "Scene.h"
extern SceneType g_TargetScene;

void LoadRanking();
void SaveRanking(int newScore);
