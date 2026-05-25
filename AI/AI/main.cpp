#include "DxLib.h"
#include "SceneManager.h"
#include "GameTypes.h"
#include <fstream>
#include <algorithm>

GameDifficulty g_Difficulty = GameDifficulty::NORMAL;
BuffType g_Buff = BuffType::NONE;
int g_Score = 0;
std::vector<int> g_Ranking;
SceneType g_TargetScene = SceneType::TITLE;
bool g_ExitGame = false;

void LoadRanking() {
    g_Ranking.clear();
    std::ifstream ifs("ranking.txt");
    if (ifs) {
        int score;
        while (ifs >> score) {
            g_Ranking.push_back(score);
        }
    }
    std::sort(g_Ranking.rbegin(), g_Ranking.rend());
    if (g_Ranking.size() > 5) g_Ranking.resize(5);
}

void SaveRanking(int newScore) {
    g_Ranking.push_back(newScore);
    std::sort(g_Ranking.rbegin(), g_Ranking.rend());
    if (g_Ranking.size() > 5) g_Ranking.resize(5);
    
    std::ofstream ofs("ranking.txt");
    for (int s : g_Ranking) {
        ofs << s << "\n";
    }
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    
    ChangeWindowMode(FALSE);
    
    
    SetGraphMode(800, 600, 32);
    
    
    SetMainWindowText(L"繧｢繧ｯ繧ｷ繝ｧ繝ｳ繧ｲ繝ｼ繝");

    
    if (DxLib_Init() == -1) {
        return -1; 
    }

    
    SetMouseDispFlag(FALSE);

    
    SetDrawScreen(DX_SCREEN_BACK);

    LoadRanking();

    
    SceneManager* sceneManager = new SceneManager();
    sceneManager->Initialize();

    
    
    
    
    
    
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && ClearDrawScreen() == 0 && !g_ExitGame) {
        
        
        sceneManager->Update();
        
        
        sceneManager->Draw();
        
        
        ScreenFlip();
    }

    
    sceneManager->Finalize();
    delete sceneManager;

    
    DxLib_End();
    return 0;
}
