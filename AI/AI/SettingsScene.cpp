#include "SettingsScene.h"
#include <cmath>

void SettingsScene::Initialize() {
    fontHandle = CreateFontToHandle(L"Arial Black", 32, 8, DX_FONTTYPE_ANTIALIASING_EDGE);
    smallFontHandle = CreateFontToHandle(L"Consolas", 18, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
    cursorIndex = 0;
    keyWaitTimer = 30;
    timer = 0;
    isWaitingForKey = false;
}

SceneType SettingsScene::Update() {
    if (keyWaitTimer > 0) keyWaitTimer--;
    timer++;

    if (isWaitingForKey) {
        char keys[256];
        GetHitKeyStateAll(keys);
        for (int i = 0; i < 256; i++) {
            if (keys[i] == 1) {
                if (i == KEY_INPUT_ESCAPE) {
                    isWaitingForKey = false;
                    keyWaitTimer = 10;
                    PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
                    break;
                }
                
                int* targetKey = nullptr;
                if (cursorIndex == 2) targetKey = &g_KeyMoveUp;
                else if (cursorIndex == 3) targetKey = &g_KeyMoveDown;
                else if (cursorIndex == 4) targetKey = &g_KeyMoveLeft;
                else if (cursorIndex == 5) targetKey = &g_KeyMoveRight;
                else if (cursorIndex == 6) targetKey = &g_KeyAltMoveUp;
                else if (cursorIndex == 7) targetKey = &g_KeyAltMoveDown;
                else if (cursorIndex == 8) targetKey = &g_KeyAttack;
                else if (cursorIndex == 9) targetKey = &g_KeyBarrier;
                else if (cursorIndex == 10) targetKey = &g_KeyPause;

                if (targetKey != nullptr) {
                    *targetKey = i;
                    isWaitingForKey = false;
                    keyWaitTimer = 10;
                    PlaySoundFile(L"C:\\Windows\\Media\\Windows Ding.wav", DX_PLAYTYPE_BACK);
                }
                break;
            }
        }
        return SceneType::SETTINGS;
    }

    if (keyWaitTimer == 0) {
        if (CheckHitKey(KEY_INPUT_UP)) {
            cursorIndex--;
            if (cursorIndex < 0) cursorIndex = 10;
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            keyWaitTimer = 10;
        }
        if (CheckHitKey(KEY_INPUT_DOWN)) {
            cursorIndex++;
            if (cursorIndex > 10) cursorIndex = 0;
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            keyWaitTimer = 10;
        }

        if (CheckHitKey(KEY_INPUT_LEFT)) {
            if (cursorIndex == 0 && g_BGMVolume > 0) g_BGMVolume -= 5;
            if (cursorIndex == 1 && g_SEVolume > 0) g_SEVolume -= 5;
            if (g_BGMVolume < 0) g_BGMVolume = 0;
            if (g_SEVolume < 0) g_SEVolume = 0;
            SetVolumeMusic(g_BGMVolume);
            SetVolumeSoundFile(g_SEVolume);
            if (cursorIndex == 1) PlaySoundFile(L"C:\\Windows\\Media\\Windows Default.wav", DX_PLAYTYPE_BACK);
            keyWaitTimer = 5;
        }
        if (CheckHitKey(KEY_INPUT_RIGHT)) {
            if (cursorIndex == 0 && g_BGMVolume < 255) g_BGMVolume += 5;
            if (cursorIndex == 1 && g_SEVolume < 255) g_SEVolume += 5;
            if (g_BGMVolume > 255) g_BGMVolume = 255;
            if (g_SEVolume > 255) g_SEVolume = 255;
            SetVolumeMusic(g_BGMVolume);
            SetVolumeSoundFile(g_SEVolume);
            if (cursorIndex == 1) PlaySoundFile(L"C:\\Windows\\Media\\Windows Default.wav", DX_PLAYTYPE_BACK);
            keyWaitTimer = 5;
        }

        if (CheckHitKey(KEY_INPUT_RETURN)) {
            if (cursorIndex >= 2 && cursorIndex <= 10) {
                isWaitingForKey = true;
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Default.wav", DX_PLAYTYPE_BACK);
                keyWaitTimer = 10;
            } else {
                SaveSettings();
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
                return SceneType::TITLE;
            }
        }
        
        if (CheckHitKey(KEY_INPUT_ESCAPE)) {
            SaveSettings();
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
            return SceneType::TITLE;
        }
    }
    return SceneType::SETTINGS;
}

void SettingsScene::Draw() {
    
    // Cool background animation
    for (int i = 0; i < 20; i++) {
        float angle = timer * 0.02f + i * 0.3f;
        int x = 400 + (int)(cos(angle) * (200 + sin(timer*0.01f)*50));
        int y = 300 + (int)(sin(angle) * (200 + cos(timer*0.01f)*50));
        SetDrawBlendMode(DX_BLENDMODE_ADD, 50);
        DrawCircle(x, y, 5 + (i % 5), GetColor(0, 255, 255), TRUE);
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    float slideIn = (timer < 30) ? (30 - timer) / 30.0f : 0.0f;
    int offsetX = (int)(slideIn * 400);

    DrawStringToHandle(300, 50 - offsetX, L"SETTINGS", GetColor(0, 255, 255), fontHandle);

    int c1 = (cursorIndex == 0) ? GetColor(255, 255, 0) : GetColor(150, 150, 150);
    int c2 = (cursorIndex == 1) ? GetColor(255, 255, 0) : GetColor(150, 150, 150);

    DrawFormatStringToHandle(80 - offsetX, 200, c1, fontHandle, L"BGM < %3d >", (int)(g_BGMVolume * 100 / 255));
    DrawFormatStringToHandle(80 + offsetX, 280, c2, fontHandle, L"SE  < %3d >", (int)(g_SEVolume * 100 / 255));

    // Input Info Box
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(180 * (1.0f - slideIn)));
    DrawBox(400, 100, 780, 500, GetColor(0, 20, 40), TRUE);
    DrawBox(400, 100, 780, 500, GetColor(0, 255, 255), FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    int alphaText = (int)(255 * (1.0f - slideIn));
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaText);
    DrawStringToHandle(500, 115, L"- CONTROLS -", GetColor(255, 200, 0), smallFontHandle);

    const wchar_t* names[] = {
        L"MOVE UP", L"MOVE DOWN", L"MOVE LEFT", L"MOVE RIGHT",
        L"ALT M. UP", L"ALT M. DOWN", L"ATTACK", L"BARRIER", L"PAUSE"
    };
    int* keys[] = {
        &g_KeyMoveUp, &g_KeyMoveDown, &g_KeyMoveLeft, &g_KeyMoveRight,
        &g_KeyAltMoveUp, &g_KeyAltMoveDown, &g_KeyAttack, &g_KeyBarrier, &g_KeyPause
    };

    for (int i = 0; i < 9; i++) {
        int color = (cursorIndex == i + 2) ? GetColor(255, 255, 0) : GetColor(200, 255, 255);
        if (isWaitingForKey && cursorIndex == i + 2) {
            color = (timer % 20 < 10) ? GetColor(255, 0, 0) : GetColor(255, 255, 0);
            DrawFormatStringToHandle(420, 155 + i * 35, color, smallFontHandle, L"%-15ls: PRESS ANY KEY", names[i]);
        } else {
            DrawFormatStringToHandle(420, 155 + i * 35, color, smallFontHandle, L"%-15ls: [ %ls ]", names[i], GetKeyName(*(keys[i])));
        }
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    int pulse = 150 + (int)(sin(GetNowCount() * 0.01f) * 105);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, pulse);
    DrawStringToHandle(210, 520 + offsetX, L"PRESS ENTER TO SAVE AND RETURN", GetColor(255, 255, 255), fontHandle);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SettingsScene::Finalize() {
    DeleteFontToHandle(fontHandle);
    DeleteFontToHandle(smallFontHandle);
}
