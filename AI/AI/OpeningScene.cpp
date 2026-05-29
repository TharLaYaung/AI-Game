#include "OpeningScene.h"
#include "DxLib.h"
#include <math.h>

void OpeningScene::Initialize() {
    timer = 0;
    titleFontHandle = CreateFontToHandle(L"Arial Black", 60, 7, DX_FONTTYPE_ANTIALIASING_EDGE);
    cyberFontHandle = CreateFontToHandle(L"Arial Black", 60, 2, DX_FONTTYPE_ANTIALIASING_EDGE);
    smallFontHandle = CreateFontToHandle(L"Arial Black", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
    bgImageHandle = LoadGraph(L"Resources\\img\\bg_cyberpunk.png");
    
    
    PlaySoundFile(L"Resources\\SE\\Windows Logon.wav", DX_PLAYTYPE_BACK);
    
    
    PlayMusic(L"Resources\\BGM\\Drifting_At_The_Edge.mp3", DX_PLAYTYPE_LOOP);
}

SceneType OpeningScene::Update() {
    timer++;
    
    
    if (timer > 300 || CheckHitKey(KEY_INPUT_RETURN) == 1 || CheckHitKey(KEY_INPUT_SPACE) == 1 || (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
        PlaySoundFile(L"Resources\\SE\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
        return SceneType::TITLE; 
    }
    
    return SceneType::OPENING;
}

void OpeningScene::Draw() {
    
    DrawExtendGraph(0, 0, 800, 600, bgImageHandle, TRUE);
    
    
    int gridSize = 40;
    int offset = (timer * 2) % gridSize;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
    for (int i = 0; i < 800; i += gridSize) {
        DrawLine(i, 0, i, 600, GetColor(0, 255, 255));
        DrawLine(0, i + offset, 800, i + offset, GetColor(0, 255, 255));
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    
    int alpha = (timer < 60) ? 255 - (timer * 4) : 150;
    if (alpha < 150) alpha = 150;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    DrawBox(0, 0, 800, 600, GetColor(0, 5, 20), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    
    if (timer > 60) {
        int glitchOffset = 0;
        int r = 0, g = 255, b = 255;
        
        
        if (timer % 60 > 50) {
            glitchOffset = GetRand(20) - 10;
            r = 255; g = 0; b = 255;
            
            SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
            DrawBox(0, 250 + GetRand(100), 800, 280 + GetRand(50), GetColor(255, 255, 255), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
        DrawStringToHandle(145 + glitchOffset, 220, L"CYBORG BATTLE", GetColor(255, 0, 255), cyberFontHandle);
        DrawStringToHandle(155 - glitchOffset, 220, L"CYBORG BATTLE", GetColor(0, 255, 255), cyberFontHandle);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        
        
        DrawStringToHandle(150, 220, L"CYBORG BATTLE", GetColor(r, g, b), titleFontHandle);
    }
    
    
    if (timer > 30 && timer < 180) {
        DrawStringToHandle(10, 10, L"SYSTEM STARTING...", GetColor(0, 255, 0), smallFontHandle);
    }
    if (timer > 80 && timer < 180) {
        DrawStringToHandle(10, 40, L"LOADING ASSETS...", GetColor(0, 255, 0), smallFontHandle);
    }
    if (timer > 130 && timer < 180) {
        DrawStringToHandle(10, 70, L"ALL SYSTEMS GREEN...", GetColor(0, 255, 0), smallFontHandle);
    }

    
    if (timer > 180) {
        int blink = (int)(sinf(timer * 0.1f) * 127) + 128;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, blink);
        DrawStringToHandle(250, 450, L"PRESS ANY KEY TO START", GetColor(0, 255, 255), smallFontHandle);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

void OpeningScene::Finalize() {
    DeleteFontToHandle(titleFontHandle);
    DeleteFontToHandle(cyberFontHandle);
    DeleteFontToHandle(smallFontHandle);
    DeleteGraph(bgImageHandle);
}
