#include "TutorialScene.h"
#include <cmath>

void TutorialScene::Initialize() {
    fontHandle = CreateFontToHandle(L"メイリオ", 32, 8, DX_FONTTYPE_ANTIALIASING_EDGE);
    smallFontHandle = CreateFontToHandle(L"メイリオ", 20, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
    timer = 0;
}

SceneType TutorialScene::Update() {
    timer++;
    
    // プレイヤーが意図せずスキップしてしまわないように、最低限の表示猶予フレームを確保
    if (timer > 30) {
        if (CheckHitKey(KEY_INPUT_RETURN) || CheckHitKey(KEY_INPUT_SPACE) || (GetMouseInput() & MOUSE_INPUT_LEFT)) {
            PlaySoundFile(L"Resources\\SE\\Windows Default.wav", DX_PLAYTYPE_BACK);
            return SceneType::MAIN;
        }
    }
    
    return SceneType::TUTORIAL;
}

void TutorialScene::Draw() {
    // 視覚的な動きを持たせるため背景演出
    for (int i = 0; i < 20; i++) {
        float angle = timer * 0.02f + i * 0.3f;
        int x = 400 + (int)(cosf(angle) * (200 + sinf(timer*0.01f)*50));
        int y = 300 + (int)(sinf(angle) * (200 + cosf(timer*0.01f)*50));
        SetDrawBlendMode(DX_BLENDMODE_ADD, 50);
        DrawCircle(x, y, 5 + (i % 5), GetColor(0, 255, 0), TRUE);
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    float slideIn = (timer < 30) ? (30 - timer) / 30.0f : 0.0f;
    int offsetY = (int)(slideIn * 400);

    DrawStringToHandle(250, 50 - offsetY, L"ー 操作方法 ー", GetColor(0, 255, 0), fontHandle);

    // Box
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(180 * (1.0f - slideIn)));
    DrawBox(100, 120, 700, 480, GetColor(0, 30, 20), TRUE);
    DrawBox(100, 120, 700, 480, GetColor(0, 255, 100), FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    int alphaText = (int)(255 * (1.0f - slideIn));
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaText);
    
    const wchar_t* names[] = {
        L"上移動", L"下移動", L"左移動", L"右移動",
        L"上移動(Z軸)", L"下移動(Z軸)", L"攻撃", L"バリア", L"ポーズ"
    };
    int* keys[] = {
        &g_KeyMoveUp, &g_KeyMoveDown, &g_KeyMoveLeft, &g_KeyMoveRight,
        &g_KeyAltMoveUp, &g_KeyAltMoveDown, &g_KeyAttack, &g_KeyBarrier, &g_KeyPause
    };

    for (int i = 0; i < 9; i++) {
        if (i == 6) {
            DrawFormatStringToHandle(250, 140 + i * 35, GetColor(200, 255, 200), smallFontHandle, L"%-15ls: [ 左クリック ]", names[i]);
        } else if (i == 7) {
            DrawFormatStringToHandle(250, 140 + i * 35, GetColor(200, 255, 200), smallFontHandle, L"%-15ls: [ 右クリック ]", names[i]);
        } else {
            DrawFormatStringToHandle(250, 140 + i * 35, GetColor(200, 255, 200), smallFontHandle, L"%-15ls: [ %ls ]", names[i], GetKeyName(*(keys[i])));
        }
    }
    
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    int pulse = 150 + (int)(sinf(GetNowCount() * 0.01f) * 105);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, pulse);
    DrawStringToHandle(140, 520 + offsetY, L"ENTERまたは左クリックでバトル開始", GetColor(255, 255, 255), fontHandle);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TutorialScene::Finalize() {
    DeleteFontToHandle(fontHandle);
    DeleteFontToHandle(smallFontHandle);
}
