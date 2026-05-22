#include "RankingScene.h"
#include "GameTypes.h"
#include "DxLib.h"

void RankingScene::Initialize() {}

SceneType RankingScene::Update() {
    // エンターキーでタイトルに戻る
    if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
        return SceneType::TITLE;
    }
    return SceneType::RANKING;
}

void RankingScene::Draw() {
    DrawStringToHandle(250, 150, L"=== TOP RANKING ===", GetColor(255, 215, 0), CreateFontToHandle(L"Arial Black", 30, 3, DX_FONTTYPE_ANTIALIASING_EDGE));
    
    int subFont = CreateFontToHandle(L"Arial Black", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
    for (size_t i = 0; i < g_Ranking.size(); i++) {
        DrawFormatStringToHandle(300, 220 + (int)i * 40, GetColor(255, 255, 255), subFont, L"%d. %06d pts", (int)i + 1, g_Ranking[i]);
    }
    if (g_Ranking.empty()) {
        DrawStringToHandle(300, 220, L"NO RECORDS YET", GetColor(150, 150, 150), subFont);
    }
    
    DrawString(250, 400, L"PRESS ENTER TO RETURN", GetColor(255, 255, 0));
}

void RankingScene::Finalize() {}
