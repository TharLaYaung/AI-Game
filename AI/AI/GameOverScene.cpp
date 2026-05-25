#include "GameOverScene.h"
#include "GameTypes.h"
#include "DxLib.h"
#include <math.h>

void GameOverScene::Initialize() {
    timer = 0;
    mainFontHandle = CreateFontToHandle(L"Arial Black", 60, 5, DX_FONTTYPE_ANTIALIASING_EDGE);
    subFontHandle = CreateFontToHandle(L"Arial Black", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
    bgImageHandle = LoadGraph(L"bg_cyberpunk.png");
    SaveRanking(g_Score);
    
    
    PlayMusic(L"C:\\Windows\\Media\\Ring06.wav", DX_PLAYTYPE_LOOP);
}

SceneType GameOverScene::Update() {
    timer++;
    if (timer < 30) return SceneType::GAMEOVER; 
    
    if (CheckHitKey(KEY_INPUT_RETURN) == 1 || (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
        return SceneType::TITLE;
    }
    return SceneType::GAMEOVER;
}

void GameOverScene::Draw() {
    DrawExtendGraph(0, 0, 800, 600, bgImageHandle, TRUE);
    
    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(80, 120, 720, 480, GetColor(30, 0, 0), TRUE);
    DrawBox(80, 120, 720, 480, GetColor(255, 0, 0), FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    int glitchX = GetRand(10) - 5;
    int glitchY = GetRand(10) - 5;
    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
    DrawStringToHandle(220 + glitchX, 140 + glitchY, L"GAME OVER", GetColor(255, 0, 0), mainFontHandle);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawStringToHandle(220, 140, L"GAME OVER", GetColor(200, 0, 0), mainFontHandle);

    DrawFormatStringToHandle(270, 220, GetColor(255, 215, 0), subFontHandle, L"FINAL SCORE: %06d", g_Score);
    
    DrawStringToHandle(300, 280, L"--- TOP RANKING ---", GetColor(255, 100, 100), subFontHandle);
    for (size_t i = 0; i < g_Ranking.size(); i++) {
        int color = (g_Ranking[i] == g_Score) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);
        DrawFormatStringToHandle(320, 320 + (int)i * 30, color, subFontHandle, L"%d. %06d", (int)i + 1, g_Ranking[i]);
    }

    if ((timer / 30) % 2 == 0) {
        DrawStringToHandle(230, 520, L"PRESS ENTER TO RETURN", GetColor(255, 255, 0), subFontHandle);
    }
}

void GameOverScene::Finalize() {
    DeleteFontToHandle(mainFontHandle);
    DeleteFontToHandle(subFontHandle);
    DeleteGraph(bgImageHandle);
}
