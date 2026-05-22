#include "CharacterSelectScene.h"
#include "GameTypes.h"
#include "DxLib.h"
#include <math.h>

void CharacterSelectScene::Initialize() {
    timer = 0;
    mainFontHandle = CreateFontToHandle(L"Arial Black", 50, 5, DX_FONTTYPE_ANTIALIASING_EDGE);
    subFontHandle = CreateFontToHandle(L"Arial Black", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
    bgImageHandle = LoadGraph(L"bg_cyberpunk.png");
}

SceneType CharacterSelectScene::Update() {
    timer++;
    
    if (CheckHitKey(KEY_INPUT_UP) == 1 || CheckHitKey(KEY_INPUT_W) == 1) {
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
        if (g_Buff == BuffType::LASER) g_Buff = BuffType::NONE;
        else if (g_Buff == BuffType::BOMB) g_Buff = BuffType::LASER;
        else if (g_Buff == BuffType::SPEED) g_Buff = BuffType::BOMB;
        else if (g_Buff == BuffType::NONE) g_Buff = BuffType::SPEED;
        WaitTimer(150);
    }
    if (CheckHitKey(KEY_INPUT_DOWN) == 1 || CheckHitKey(KEY_INPUT_S) == 1) {
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
        if (g_Buff == BuffType::NONE) g_Buff = BuffType::LASER;
        else if (g_Buff == BuffType::LASER) g_Buff = BuffType::BOMB;
        else if (g_Buff == BuffType::BOMB) g_Buff = BuffType::SPEED;
        else if (g_Buff == BuffType::SPEED) g_Buff = BuffType::NONE;
        WaitTimer(150);
    }
    
    if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
        return SceneType::MAIN;
    }
    return SceneType::CHARACTER_SELECT;
}

void CharacterSelectScene::Draw() {
    DrawExtendGraph(0, 0, 800, 600, bgImageHandle, TRUE);
    
    // Draw dark semi-transparent panel for readability
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(80, 60, 720, 560, GetColor(10, 30, 0), TRUE);
    DrawBox(80, 60, 720, 560, GetColor(0, 255, 100), FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    int yOffset = (int)(sin(timer * 0.05f) * 10);
    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawStringToHandle(145, 85 + yOffset, L"SELECT INITIAL BUFF", GetColor(0, 255, 0), mainFontHandle);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawStringToHandle(140, 80 + yOffset, L"SELECT INITIAL BUFF", GetColor(150, 255, 150), mainFontHandle);
    
    int noneColor = (g_Buff == BuffType::NONE) ? GetColor(255, 255, 255) : GetColor(100, 100, 100);
    int laserColor = (g_Buff == BuffType::LASER) ? GetColor(0, 255, 255) : GetColor(100, 100, 100);
    int bombColor = (g_Buff == BuffType::BOMB) ? GetColor(255, 100, 0) : GetColor(100, 100, 100);
    int speedColor = (g_Buff == BuffType::SPEED) ? GetColor(255, 255, 0) : GetColor(100, 100, 100);
    
    DrawStringToHandle(250, 200, L"[1] NONE (STANDARD)", noneColor, subFontHandle);
    DrawStringToHandle(250, 250, L"[2] PIERCING LASER", laserColor, subFontHandle);
    DrawStringToHandle(250, 300, L"[3] BOMB EXPLOSION", bombColor, subFontHandle);
    DrawStringToHandle(250, 350, L"[4] SPEED BOOST", speedColor, subFontHandle);
    
    if (g_Buff == BuffType::NONE) DrawStringToHandle(200, 200, L">>", noneColor, subFontHandle);
    if (g_Buff == BuffType::LASER) DrawStringToHandle(200, 250, L">>", laserColor, subFontHandle);
    if (g_Buff == BuffType::BOMB) DrawStringToHandle(200, 300, L">>", bombColor, subFontHandle);
    if (g_Buff == BuffType::SPEED) DrawStringToHandle(200, 350, L">>", speedColor, subFontHandle);
    
    if ((timer / 30) % 2 == 0) {
        DrawStringToHandle(230, 480, L"PRESS ENTER TO START", GetColor(0, 255, 255), subFontHandle);
    }
}

void CharacterSelectScene::Finalize() {
    DeleteFontToHandle(mainFontHandle);
    DeleteFontToHandle(subFontHandle);
    DeleteGraph(bgImageHandle);
}
