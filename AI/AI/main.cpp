#include "DxLib.h"
#include "SceneManager.h"
#include "GameTypes.h"
#include <fstream>
#include <algorithm>

GameDifficulty g_Difficulty = GameDifficulty::NORMAL;
BuffType g_Buff = BuffType::NONE;
int g_Score = 0;
std::vector<int> g_Ranking;

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

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // ウィンドウモードで起動するように設定（フルスクリーンを回避）
    ChangeWindowMode(FALSE);
    
    // 画面の解像度を設定 (例: 幅800px, 高さ600px)
    SetGraphMode(800, 600, 32);
    
    // ウィンドウのタイトルを設定
    SetMainWindowText(L"アクションゲーム");

    // DXライブラリの初期化処理
    if (DxLib_Init() == -1) {
        return -1; // エラーが起きたら強制終了
    }

    // 描画先を「裏画面」に設定（画面のちらつきを防止するための必須設定）
    SetDrawScreen(DX_SCREEN_BACK);

    LoadRanking();

    // シーンマネージャー（画面遷移を管理するもの）を作成して初期化
    SceneManager* sceneManager = new SceneManager();
    sceneManager->Initialize();

    // ---------------------------------------------------------
    // メインループ
    // ProcessMessage() == 0 : Windowsのメッセージ処理（ウィンドウを閉じられたか等）
    // CheckHitKey(KEY_INPUT_ESCAPE) == 0 : エスケープキーが押されていないか確認（押されたら終了）
    // ClearDrawScreen() == 0 : 裏画面の描画を一旦真っ黒にクリアする
    // ---------------------------------------------------------
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && ClearDrawScreen() == 0) {
        
        // 1. シーンの更新（プレイヤーの移動計算や、ボスの攻撃判定など）
        sceneManager->Update();
        
        // 2. シーンの描画（計算結果をもとに画像や図形を裏画面に描画）
        sceneManager->Draw();
        
        // 3. 裏画面の内容を表画面に反映（ここで実際にモニターに表示される）
        ScreenFlip();
    }

    // 終了処理（メモリの解放など）
    sceneManager->Finalize();
    delete sceneManager;

    // DXライブラリの使用終了処理
    DxLib_End();
    return 0;
}
