#include "RankingScene.h"
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
    DrawString(300, 200, L"=== RANKING ===", GetColor(255, 215, 0)); // ゴールド色
    DrawString(300, 250, L"1st: 10000 pts", GetColor(255, 255, 255));
    DrawString(300, 280, L"2nd:  8000 pts", GetColor(255, 255, 255));
    DrawString(300, 310, L"3rd:  5000 pts", GetColor(255, 255, 255));
    
    DrawString(250, 400, L"Enterキーでタイトルに戻る", GetColor(255, 255, 0));
}

void RankingScene::Finalize() {}
