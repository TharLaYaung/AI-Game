#include "TitleScene.h"
#include "GameTypes.h"
#include "DxLib.h"
#include <math.h>

void TitleScene::Initialize() {
    timer = 0;
    menuIndex = 0;
    showSettings = false;
    inputCooldown = 0;
    
    titleFontHandle = CreateFontToHandle(L"Impact", 64, 5, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
    subFontHandle = CreateFontToHandle(L"Arial Black", 32, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
    difficultyFontHandle = CreateFontToHandle(L"Consolas", 28, 4, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
    bgImageHandle = LoadGraph(L"bg_cyberpunk.png");
}

SceneType TitleScene::Update() {
    timer++;
    if (timer < 30) return SceneType::TITLE;
    
    if (inputCooldown > 0) inputCooldown--;

    if (showSettings) {
        if (inputCooldown == 0 && (CheckHitKey(KEY_INPUT_RETURN) || CheckHitKey(KEY_INPUT_ESCAPE) || (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)) {
            showSettings = false;
            inputCooldown = 15;
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
        }
        return SceneType::TITLE;
    }

    if (inputCooldown == 0) {
        int mouseX, mouseY;
        GetMousePoint(&mouseX, &mouseY);
        
        for (int i = 0; i < 3; i++) {
            int y = 280 + i * 70;
            if (mouseX >= 250 && mouseX <= 600 && mouseY >= y && mouseY <= y + 50) {
                if (menuIndex != i) {
                    menuIndex = i;
                    PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
                }
            }
        }

        if (CheckHitKey(KEY_INPUT_UP) == 1 || CheckHitKey(KEY_INPUT_W) == 1) {
            menuIndex--;
            if (menuIndex < 0) menuIndex = 2;
            inputCooldown = 15;
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
        }
        else if (CheckHitKey(KEY_INPUT_DOWN) == 1 || CheckHitKey(KEY_INPUT_S) == 1) {
            menuIndex++;
            if (menuIndex > 2) menuIndex = 0;
            inputCooldown = 15;
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
        }
    }
    
    if (inputCooldown == 0 && (CheckHitKey(KEY_INPUT_RETURN) == 1 || (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)) {
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
        inputCooldown = 15;
        if (menuIndex == 0) return SceneType::CHARACTER_SELECT;
        if (menuIndex == 1) {
            showSettings = true;
            return SceneType::TITLE;
        }
        if (menuIndex == 2) {
            g_ExitGame = true;
            return SceneType::TITLE;
        }
    }
    return SceneType::TITLE; 
}

void TitleScene::Draw() {
    DrawExtendGraph(0, 0, 800, 600, bgImageHandle, TRUE);
    
    int gridSize = 40;
    int offset = timer % gridSize;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
    for (int i = 0; i < 800; i += gridSize) {
        DrawLine(i, 0, i, 600, GetColor(0, 255, 255));
        DrawLine(0, i + offset, 800, i + offset, GetColor(0, 255, 255));
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(80, 60, 720, 560, GetColor(0, 10, 30), TRUE);
    DrawBox(80, 60, 720, 560, GetColor(0, 255, 255), FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    int yOffset = (int)(sin(timer * 0.05f) * 10);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawStringToHandle(155, 105 + yOffset, L"CYBORG BATTLE", GetColor(255, 0, 255), titleFontHandle);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawStringToHandle(150, 100 + yOffset, L"CYBORG BATTLE", GetColor(0, 255, 255), titleFontHandle);
    
    if (showSettings) {
        
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
        DrawBox(100, 200, 700, 500, GetColor(0, 20, 50), TRUE);
        DrawBox(100, 200, 700, 500, GetColor(0, 255, 255), FALSE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        
        DrawStringToHandle(300, 220, L"CONTROLS", GetColor(0, 255, 255), subFontHandle);
        DrawStringToHandle(180, 280, L"W A S D   : Move Spacecraft", GetColor(255, 255, 255), difficultyFontHandle);
        DrawStringToHandle(180, 330, L"SPACE     : Shoot Weapon", GetColor(255, 255, 255), difficultyFontHandle);
        DrawStringToHandle(180, 380, L"SHIFT     : Toggle Barrier Mode", GetColor(255, 255, 255), difficultyFontHandle);
        DrawStringToHandle(220, 450, L"CLICK OR ENTER TO RETURN", GetColor(255, 255, 0), difficultyFontHandle);
        return;
    }

    
    const wchar_t* options[3] = { L"START GAME", L"SETTINGS", L"EXIT" };
    for (int i = 0; i < 3; i++) {
        int y = 280 + i * 70;
        if (menuIndex == i) {
            
            int glowAlpha = 150 + (int)(sin(timer * 0.1f) * 100);
            SetDrawBlendMode(DX_BLENDMODE_ADD, glowAlpha);
            DrawStringToHandle(290, y, options[i], GetColor(0, 255, 255), subFontHandle);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            
            
            int cursorOffset = (int)(sin(timer * 0.2f) * 5);
            DrawStringToHandle(250 + cursorOffset, y, L">", GetColor(0, 255, 255), subFontHandle);
        } else {
            DrawStringToHandle(290, y, options[i], GetColor(100, 100, 100), subFontHandle);
        }
    }
}

void TitleScene::Finalize() {
    DeleteFontToHandle(titleFontHandle);
    DeleteFontToHandle(subFontHandle);
    DeleteFontToHandle(difficultyFontHandle);
    DeleteGraph(bgImageHandle);
}
