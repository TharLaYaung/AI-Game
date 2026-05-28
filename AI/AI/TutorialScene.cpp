#include "TutorialScene.h"
#include <cmath>

void TutorialScene::Initialize() {
    fontHandle = CreateFontToHandle(L"Arial Black", 32, 8, DX_FONTTYPE_ANTIALIASING_EDGE);
    smallFontHandle = CreateFontToHandle(L"Consolas", 20, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
    timer = 0;
}

SceneType TutorialScene::Update() {
    timer++;
    
    // Wait a little before allowing to skip
    if (timer > 30) {
        if (CheckHitKey(KEY_INPUT_RETURN) || CheckHitKey(KEY_INPUT_SPACE) || (GetMouseInput() & MOUSE_INPUT_LEFT)) {
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Default.wav", DX_PLAYTYPE_BACK);
            return SceneType::MAIN;
        }
    }
    
    return SceneType::TUTORIAL;
}

void TutorialScene::Draw() {
    // Cool background animation
    for (int i = 0; i < 20; i++) {
        float angle = timer * 0.02f + i * 0.3f;
        int x = 400 + (int)(cos(angle) * (200 + sin(timer*0.01f)*50));
        int y = 300 + (int)(sin(angle) * (200 + cos(timer*0.01f)*50));
        SetDrawBlendMode(DX_BLENDMODE_ADD, 50);
        DrawCircle(x, y, 5 + (i % 5), GetColor(0, 255, 0), TRUE);
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    float slideIn = (timer < 30) ? (30 - timer) / 30.0f : 0.0f;
    int offsetY = (int)(slideIn * 400);

    DrawStringToHandle(250, 50 - offsetY, L"- HOW TO PLAY -", GetColor(0, 255, 0), fontHandle);

    // Box
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(180 * (1.0f - slideIn)));
    DrawBox(100, 120, 700, 480, GetColor(0, 30, 20), TRUE);
    DrawBox(100, 120, 700, 480, GetColor(0, 255, 100), FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    int alphaText = (int)(255 * (1.0f - slideIn));
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaText);
    
    const wchar_t* names[] = {
        L"MOVE UP", L"MOVE DOWN", L"MOVE LEFT", L"MOVE RIGHT",
        L"ALT M. UP", L"ALT M. DOWN", L"ATTACK", L"BARRIER", L"PAUSE"
    };
    int* keys[] = {
        &g_KeyMoveUp, &g_KeyMoveDown, &g_KeyMoveLeft, &g_KeyMoveRight,
        &g_KeyAltMoveUp, &g_KeyAltMoveDown, &g_KeyAttack, &g_KeyBarrier, &g_KeyPause
    };

    for (int i = 0; i < 9; i++) {
        DrawFormatStringToHandle(250, 140 + i * 35, GetColor(200, 255, 200), smallFontHandle, L"%-15ls: [ %ls ]", names[i], GetKeyName(*(keys[i])));
    }
    
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    int pulse = 150 + (int)(sin(GetNowCount() * 0.01f) * 105);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, pulse);
    DrawStringToHandle(120, 520 + offsetY, L"PRESS ENTER OR CLICK TO START BATTLE", GetColor(255, 255, 255), fontHandle);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TutorialScene::Finalize() {
    DeleteFontToHandle(fontHandle);
    DeleteFontToHandle(smallFontHandle);
}
