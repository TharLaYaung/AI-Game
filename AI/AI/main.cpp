#include "DxLib.h"
#include "SceneManager.h"
#include "GameTypes.h"
#include <fstream>
#include <algorithm>

GameDifficulty g_Difficulty = GameDifficulty::NORMAL;
std::map<BuffType, int> g_Buffs;
int g_CurrentStage = 1;
int g_Score = 0;
int g_ClearTime = 0;
int g_BGMVolume = 255;
int g_SEVolume = 255;
std::vector<RankRecord> g_Ranking;
SceneType g_TargetScene = SceneType::TITLE;
bool g_ExitGame = false;
bool g_IsFirstRun = true;

int g_KeyMoveUp = KEY_INPUT_W;
int g_KeyMoveDown = KEY_INPUT_S;
int g_KeyMoveLeft = KEY_INPUT_A;
int g_KeyMoveRight = KEY_INPUT_D;
int g_KeyAltMoveUp = KEY_INPUT_Q;
int g_KeyAltMoveDown = KEY_INPUT_E;
int g_KeyAttack = KEY_INPUT_SPACE;
int g_KeyBarrier = KEY_INPUT_LSHIFT;
int g_KeyPause = KEY_INPUT_P;

const wchar_t* GetKeyName(int keyCode) {
    switch (keyCode) {
        case KEY_INPUT_W: return L"W";
        case KEY_INPUT_A: return L"A";
        case KEY_INPUT_S: return L"S";
        case KEY_INPUT_D: return L"D";
        case KEY_INPUT_Q: return L"Q";
        case KEY_INPUT_E: return L"E";
        case KEY_INPUT_SPACE: return L"SPACE";
        case KEY_INPUT_LSHIFT: return L"LSHIFT";
        case KEY_INPUT_RSHIFT: return L"RSHIFT";
        case KEY_INPUT_P: return L"P";
        case KEY_INPUT_ESCAPE: return L"ESC";
        case KEY_INPUT_UP: return L"UP";
        case KEY_INPUT_DOWN: return L"DOWN";
        case KEY_INPUT_LEFT: return L"LEFT";
        case KEY_INPUT_RIGHT: return L"RIGHT";
        case KEY_INPUT_RETURN: return L"ENTER";
        case KEY_INPUT_Z: return L"Z";
        case KEY_INPUT_X: return L"X";
        case KEY_INPUT_C: return L"C";
        case KEY_INPUT_V: return L"V";
        case KEY_INPUT_B: return L"B";
        case KEY_INPUT_N: return L"N";
        case KEY_INPUT_M: return L"M";
        case KEY_INPUT_F: return L"F";
        case KEY_INPUT_G: return L"G";
        case KEY_INPUT_H: return L"H";
        case KEY_INPUT_J: return L"J";
        case KEY_INPUT_K: return L"K";
        case KEY_INPUT_L: return L"L";
        case KEY_INPUT_R: return L"R";
        case KEY_INPUT_T: return L"T";
        case KEY_INPUT_Y: return L"Y";
        case KEY_INPUT_U: return L"U";
        case KEY_INPUT_I: return L"I";
        case KEY_INPUT_O: return L"O";
        case KEY_INPUT_1: return L"1";
        case KEY_INPUT_2: return L"2";
        case KEY_INPUT_3: return L"3";
        case KEY_INPUT_4: return L"4";
        case KEY_INPUT_5: return L"5";
        case KEY_INPUT_6: return L"6";
        case KEY_INPUT_7: return L"7";
        case KEY_INPUT_8: return L"8";
        case KEY_INPUT_9: return L"9";
        case KEY_INPUT_0: return L"0";
        default: return L"KEY";
    }
}

void LoadRanking() {
    g_Ranking.clear();
    std::ifstream ifs("ranking.txt");
    if (ifs) {
        std::string name;
        int score, clearTime;
        while (ifs >> name >> score >> clearTime) {
            g_Ranking.push_back({name, score, clearTime});
        }
    } else {
        // セーブデータが存在しない初回起動時はダミーデータで初期化し、空リストによるクラッシュを防ぐ
        g_Ranking.push_back({"CPU", 500, 99999});
        g_Ranking.push_back({"AIX", 400, 99999});
        g_Ranking.push_back({"BOT", 300, 99999});
        g_Ranking.push_back({"SYS", 200, 99999});
        g_Ranking.push_back({"NAN", 100, 99999});
    }
    std::sort(g_Ranking.begin(), g_Ranking.end(), [](const RankRecord& a, const RankRecord& b) {
        return a.score > b.score;
    });
    if (g_Ranking.size() > 5) g_Ranking.resize(5);
}

void SaveRanking(const std::string& name, int newScore) {
    g_Ranking.push_back({name, newScore, g_ClearTime});
    std::sort(g_Ranking.begin(), g_Ranking.end(), [](const RankRecord& a, const RankRecord& b) {
        return a.score > b.score;
    });
    if (g_Ranking.size() > 5) g_Ranking.resize(5);
    
    std::ofstream ofs("ranking.txt");
    for (const auto& r : g_Ranking) {
        ofs << r.name << " " << r.score << " " << r.clearTime << "\n";
    }
}

void LoadSettings() {
    g_BGMVolume = 255;
    g_SEVolume = 255;
    g_KeyMoveUp = KEY_INPUT_W;
    g_KeyMoveDown = KEY_INPUT_S;
    g_KeyMoveLeft = KEY_INPUT_A;
    g_KeyMoveRight = KEY_INPUT_D;
    g_KeyAltMoveUp = KEY_INPUT_Q;
    g_KeyAltMoveDown = KEY_INPUT_E;
    g_KeyAttack = KEY_INPUT_SPACE;
    g_KeyBarrier = KEY_INPUT_LSHIFT;
    g_KeyPause = KEY_INPUT_P;

    std::ifstream ifs("settings.txt");
    if (ifs) {
        ifs >> g_BGMVolume >> g_SEVolume;
        int up, down, left, right, aUp, aDown, atk, bar, p;
        if (ifs >> up >> down >> left >> right >> aUp >> aDown >> atk >> bar >> p) {
            g_KeyMoveUp = up;
            g_KeyMoveDown = down;
            g_KeyMoveLeft = left;
            g_KeyMoveRight = right;
            g_KeyAltMoveUp = aUp;
            g_KeyAltMoveDown = aDown;
            g_KeyAttack = atk;
            g_KeyBarrier = bar;
            g_KeyPause = p;
        }
    }
    SetVolumeMusic(g_BGMVolume);
    SetVolumeSoundFile(g_SEVolume);
}

void SaveSettings() {
    std::ofstream ofs("settings.txt");
    ofs << g_BGMVolume << " " << g_SEVolume << "\n";
    ofs << g_KeyMoveUp << " " << g_KeyMoveDown << " " << g_KeyMoveLeft << " " << g_KeyMoveRight << " ";
    ofs << g_KeyAltMoveUp << " " << g_KeyAltMoveDown << " " << g_KeyAttack << " " << g_KeyBarrier << " " << g_KeyPause << "\n";
    SetVolumeMusic(g_BGMVolume);
    SetVolumeSoundFile(g_SEVolume);
}


// Windowsアプリケーションのエントリポイント。ゲームループの制御とDxLibの初期化/終了を担う
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    
    ChangeWindowMode(FALSE);
    
    
    SetGraphMode(800, 600, 32);
    
    
    SetMainWindowText(L"アクションゲーム");

    // DxLibの初期化処理。グラフィックボード非対応等の致命的エラー時は即座にアプリを終了する
    if (DxLib_Init() == -1) {
        return -1; 
    }

    
    SetMouseDispFlag(FALSE);

    
    SetDrawScreen(DX_SCREEN_BACK);

    LoadSettings();
    LoadRanking();

    
    SceneManager* sceneManager = new SceneManager();
    sceneManager->Initialize();

    // Windows OSからのメッセージ処理(ProcessMessage)と描画同期を単一スレッドのループで管理する制約
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
