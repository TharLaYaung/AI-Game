#include "ResultScene.h"
#include "GameTypes.h"
#include "DxLib.h"
#include <math.h>

void ResultScene::Initialize() {
    timer = 0;
    mainFontHandle = CreateFontToHandle(L"Arial Black", 50, 5, DX_FONTTYPE_ANTIALIASING_EDGE);
    subFontHandle = CreateFontToHandle(L"Arial Black", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
    bgImageHandle = LoadGraph(L"bg_cyberpunk.png");
    
    PlayMusic(L"C:\\Windows\\Media\\flourish.mid", DX_PLAYTYPE_LOOP);
}

SceneType ResultScene::Update() {
    timer++;
    if (timer < 30) return SceneType::RESULT; 
    
    if (CheckHitKey(KEY_INPUT_RETURN) == 1 || (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
        return SceneType::RANKING;
    }
    return SceneType::RESULT;
}

void ResultScene::Draw() {
    DrawExtendGraph(0, 0, 800, 600, bgImageHandle, TRUE);

    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(80, 120, 720, 480, GetColor(0, 30, 0), TRUE);
    DrawBox(80, 120, 720, 480, GetColor(0, 255, 0), FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    int yOffset = (int)(sin(timer * 0.05f) * 10);
    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawStringToHandle(165, 145 + yOffset, L"SYSTEM CLEARED", GetColor(0, 0, 255), mainFontHandle);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawStringToHandle(160, 140 + yOffset, L"SYSTEM CLEARED", GetColor(0, 255, 255), mainFontHandle);

    DrawFormatStringToHandle(270, 220, GetColor(255, 215, 0), subFontHandle, L"FINAL SCORE: %06d", g_Score);
    
    DrawStringToHandle(300, 280, L"--- TOP RANKING ---", GetColor(0, 255, 255), subFontHandle);
    for (size_t i = 0; i < g_Ranking.size(); i++) {
        int color = (g_Ranking[i].score == g_Score) ? GetColor(255, 100, 100) : GetColor(255, 255, 255);
        std::string n = g_Ranking[i].name;
        std::wstring wn(n.begin(), n.end());
        DrawFormatStringToHandle(320, 320 + (int)i * 30, color, subFontHandle, L"%d. %s  %06d", (int)i + 1, wn.c_str(), g_Ranking[i].score);
    }

    if ((timer / 30) % 2 == 0) {
        DrawStringToHandle(230, 520, L"PRESS ENTER TO CONTINUE", GetColor(255, 255, 0), subFontHandle);
    }
}

void ResultScene::Finalize() {
    DeleteFontToHandle(mainFontHandle);
    DeleteFontToHandle(subFontHandle);
    DeleteGraph(bgImageHandle);
}
