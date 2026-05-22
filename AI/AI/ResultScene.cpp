#include "ResultScene.h"
#include "DxLib.h"
#include <math.h>

void ResultScene::Initialize() {
    timer = 0;
    mainFontHandle = CreateFontToHandle(L"Impact", 70, 5, DX_FONTTYPE_ANTIALIASING_EDGE);
    subFontHandle = CreateFontToHandle(L"Consolas", 30, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
}

SceneType ResultScene::Update() {
    timer++;
    if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
        return SceneType::TITLE;
    }
    return SceneType::RESULT;
}

void ResultScene::Draw() {
    int yOffset = (int)(sin(timer * 0.05f) * 10);
    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawStringToHandle(165, 205 + yOffset, L"SYSTEM CLEARED", GetColor(0, 0, 255), mainFontHandle);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawStringToHandle(160, 200 + yOffset, L"SYSTEM CLEARED", GetColor(0, 255, 255), mainFontHandle);

    if ((timer / 30) % 2 == 0) {
        DrawStringToHandle(180, 350, L"PRESS ENTER TO RETURN", GetColor(255, 255, 0), subFontHandle);
    }
}

void ResultScene::Finalize() {
    DeleteFontToHandle(mainFontHandle);
    DeleteFontToHandle(subFontHandle);
}
