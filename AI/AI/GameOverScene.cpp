#include "GameOverScene.h"
#include "DxLib.h"
#include <math.h>

void GameOverScene::Initialize() {
    timer = 0;
    mainFontHandle = CreateFontToHandle(L"Impact", 70, 5, DX_FONTTYPE_ANTIALIASING_EDGE);
    subFontHandle = CreateFontToHandle(L"Consolas", 30, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
}

SceneType GameOverScene::Update() {
    timer++;
    if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
        return SceneType::TITLE;
    }
    return SceneType::GAMEOVER;
}

void GameOverScene::Draw() {
    int glitchX = GetRand(10) - 5;
    int glitchY = GetRand(10) - 5;
    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
    DrawStringToHandle(200 + glitchX, 200 + glitchY, L"GAME OVER", GetColor(255, 0, 0), mainFontHandle);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawStringToHandle(200, 200, L"GAME OVER", GetColor(200, 0, 0), mainFontHandle);

    if ((timer / 30) % 2 == 0) {
        DrawStringToHandle(180, 350, L"PRESS ENTER TO RETURN", GetColor(255, 255, 0), subFontHandle);
    }
}

void GameOverScene::Finalize() {
    DeleteFontToHandle(mainFontHandle);
    DeleteFontToHandle(subFontHandle);
}
