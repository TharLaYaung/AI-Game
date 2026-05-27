#include "LoadingScene.h"
#include "GameTypes.h"
#include "DxLib.h"
#include <math.h>

void LoadingScene::Initialize() {
    timer = 0;
    fontHandle = CreateFontToHandle(L"Arial Black", 40, 4, DX_FONTTYPE_ANTIALIASING_EDGE);
    smallFontHandle = CreateFontToHandle(L"Arial Black", 20, 2, DX_FONTTYPE_ANTIALIASING_EDGE);
    
    type = GetRand(1);
    if (type == 0) {
        bgImageHandle = LoadGraph(L"loading_bg.png");
    } else {
        bgImageHandle = LoadGraph(L"loading_bg_2.png");
    }
    
    PlayMusic(L"C:\\Windows\\Media\\onestop.mid", DX_PLAYTYPE_LOOP);
}

SceneType LoadingScene::Update() {
    timer++;
    
    if (timer > 180) {
        return g_TargetScene;
    }
    return SceneType::LOADING;
}

void LoadingScene::Draw() {
    DrawExtendGraph(0, 0, 800, 600, bgImageHandle, TRUE);
    
    if (type == 0) {
        
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
        DrawBox(50, 50, 750, 550, GetColor(0, 10, 30), TRUE);
        DrawBox(50, 50, 750, 550, GetColor(0, 255, 255), FALSE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        
        
        int alpha = 150 + (int)(sin(timer * 0.1f) * 105);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawStringToHandle(280, 80, L"SYSTEM LOADING...", GetColor(0, 255, 255), fontHandle);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        
        
        DrawStringToHandle(100, 180, L"--- HOW TO PLAY ---", GetColor(255, 215, 0), smallFontHandle);
        DrawStringToHandle(120, 230, L"MOVE  : W, A, S, D", GetColor(255, 255, 255), smallFontHandle);
        DrawStringToHandle(120, 270, L"ATTACK: Left Click / Space", GetColor(255, 255, 255), smallFontHandle);
        DrawStringToHandle(120, 310, L"FORM  : Right Click / Enter", GetColor(255, 255, 255), smallFontHandle);
        
        DrawStringToHandle(100, 380, L"--- TIPS TO SURVIVE ---", GetColor(255, 215, 0), smallFontHandle);
        DrawStringToHandle(120, 430, L"* Reduce Boss HP to 0 to clear!", GetColor(255, 150, 150), smallFontHandle);
        DrawStringToHandle(120, 470, L"* Use BARRIER FORM to deflect enemy bullets!", GetColor(150, 200, 255), smallFontHandle);
        DrawStringToHandle(120, 510, L"* Barrier consumes gauge, use it wisely!", GetColor(150, 200, 255), smallFontHandle);
    } else {
        
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(100, 80, 700, 480, GetColor(20, 0, 30), TRUE);
        DrawBox(100, 80, 700, 480, GetColor(255, 0, 255), FALSE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        
        int alpha = 150 + (int)(sin(timer * 0.15f) * 105);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawStringToHandle(220, 110, L"INITIALIZING...", GetColor(255, 0, 255), fontHandle);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        
        
        DrawStringToHandle(140, 200, L"==== COMMAND OVERVIEW ====", GetColor(0, 255, 255), smallFontHandle);
        DrawStringToHandle(160, 250, L"Movement  -> W/A/S/D", GetColor(200, 200, 200), smallFontHandle);
        DrawStringToHandle(160, 290, L"Fire      -> Left Click / Space", GetColor(200, 200, 200), smallFontHandle);
        DrawStringToHandle(160, 330, L"Change    -> Right Click / Enter", GetColor(200, 200, 200), smallFontHandle);
        
        DrawStringToHandle(140, 390, L"==== SYSTEM NOTICES ====", GetColor(0, 255, 255), smallFontHandle);
        DrawStringToHandle(160, 430, L"Warning: Boss attacks are lethal.", GetColor(255, 100, 100), smallFontHandle);
    }
    
    
    int barX = 100;
    int barY = 540;
    int barWidth = 600;
    int barHeight = 20;
    
    
    int baseColor = (type == 0) ? GetColor(0, 50, 50) : GetColor(50, 0, 50);
    int borderColor = (type == 0) ? GetColor(0, 255, 255) : GetColor(255, 0, 255);
    
    DrawBox(barX, barY, barX + barWidth, barY + barHeight, baseColor, TRUE);
    DrawBox(barX - 2, barY - 2, barX + barWidth + 2, barY + barHeight + 2, borderColor, FALSE);
    
    
    float progress = (float)timer / 180.0f;
    if (progress > 1.0f) progress = 1.0f;
    DrawBox(barX, barY, barX + (int)(barWidth * progress), barY + barHeight, borderColor, TRUE);
}

void LoadingScene::Finalize() {
    DeleteFontToHandle(fontHandle);
    DeleteFontToHandle(smallFontHandle);
    DeleteGraph(bgImageHandle);
    StopMusic();
}
