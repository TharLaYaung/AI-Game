#include "RankingScene.h"
#include "GameTypes.h"
#include "DxLib.h"

void RankingScene::Initialize() {
    timer = 0;
    titleFontHandle = CreateFontToHandle(L"Arial Black", 40, 4, DX_FONTTYPE_ANTIALIASING_EDGE);
    subFontHandle = CreateFontToHandle(L"Arial Black", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
    
    
    PlayMusic(L"C:\\Windows\\Media\\town.mid", DX_PLAYTYPE_LOOP);
}

SceneType RankingScene::Update() {
    timer++;
    if (timer < 30) return SceneType::RANKING;
    
    if (CheckHitKey(KEY_INPUT_RETURN) == 1 || (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
        return SceneType::TITLE;
    }
    return SceneType::RANKING;
}

void RankingScene::Draw() {
    DrawStringToHandle(150, 80, L"=== TOP RANKING ===", GetColor(255, 215, 0), titleFontHandle);
    
    for (size_t i = 0; i < g_Ranking.size(); i++) {
        DrawFormatStringToHandle(250, 180 + (int)i * 50, GetColor(255, 255, 255), subFontHandle, L"%d. %06d pts", (int)i + 1, g_Ranking[i]);
    }
    if (g_Ranking.empty()) {
        DrawStringToHandle(250, 180, L"NO RECORDS YET", GetColor(150, 150, 150), subFontHandle);
    }
    
    if ((timer / 30) % 2 == 0) {
        DrawStringToHandle(230, 500, L"PRESS ENTER TO RETURN", GetColor(0, 255, 255), subFontHandle);
    }
}

void RankingScene::Finalize() {
    DeleteFontToHandle(titleFontHandle);
    DeleteFontToHandle(subFontHandle);
}
