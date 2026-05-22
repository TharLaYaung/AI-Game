#include "TitleScene.h"
#include "GameTypes.h"
#include "DxLib.h"
#include <math.h>

void TitleScene::Initialize() {
    timer = 0;
    titleFontHandle = CreateFontToHandle(L"Arial Black", 60, 5, DX_FONTTYPE_ANTIALIASING_EDGE);
    subFontHandle = CreateFontToHandle(L"Arial Black", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
    difficultyFontHandle = CreateFontToHandle(L"Arial Black", 32, 4, DX_FONTTYPE_ANTIALIASING_EDGE);
    bgImageHandle = LoadGraph(L"bg_cyberpunk.png");
    
    PlayMusic(L"C:\\Windows\\Media\\onestop.mid", DX_PLAYTYPE_LOOP);
}

SceneType TitleScene::Update() {
    timer++;
    
    if (CheckHitKey(KEY_INPUT_UP) == 1 || CheckHitKey(KEY_INPUT_W) == 1) {
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
        if (g_Difficulty == GameDifficulty::NORMAL) g_Difficulty = GameDifficulty::EASY;
        else if (g_Difficulty == GameDifficulty::HARD) g_Difficulty = GameDifficulty::NORMAL;
        WaitTimer(150);
    }
    if (CheckHitKey(KEY_INPUT_DOWN) == 1 || CheckHitKey(KEY_INPUT_S) == 1) {
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
        if (g_Difficulty == GameDifficulty::EASY) g_Difficulty = GameDifficulty::NORMAL;
        else if (g_Difficulty == GameDifficulty::NORMAL) g_Difficulty = GameDifficulty::HARD;
        WaitTimer(150);
    }
    
    if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
        return SceneType::CHARACTER_SELECT;
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

    // Draw dark semi-transparent panel for readability
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(80, 60, 720, 560, GetColor(0, 10, 30), TRUE);
    DrawBox(80, 60, 720, 560, GetColor(0, 255, 255), FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    int yOffset = (int)(sin(timer * 0.05f) * 10);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawStringToHandle(155, 105 + yOffset, L"CYBORG BATTLE", GetColor(255, 0, 255), titleFontHandle);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawStringToHandle(150, 100 + yOffset, L"CYBORG BATTLE", GetColor(0, 255, 255), titleFontHandle);
    
    DrawStringToHandle(260, 250, L"SELECT DIFFICULTY", GetColor(255, 255, 255), subFontHandle);
    
    int easyColor = (g_Difficulty == GameDifficulty::EASY) ? GetColor(0, 255, 0) : GetColor(100, 100, 100);
    int normColor = (g_Difficulty == GameDifficulty::NORMAL) ? GetColor(255, 255, 0) : GetColor(100, 100, 100);
    int hardColor = (g_Difficulty == GameDifficulty::HARD) ? GetColor(255, 0, 0) : GetColor(100, 100, 100);
    
    DrawStringToHandle(350, 320, L"EASY", easyColor, difficultyFontHandle);
    DrawStringToHandle(330, 370, L"NORMAL", normColor, difficultyFontHandle);
    DrawStringToHandle(350, 420, L"HARD", hardColor, difficultyFontHandle);
    
    if (g_Difficulty == GameDifficulty::EASY) DrawStringToHandle(300, 320, L">", easyColor, difficultyFontHandle);
    if (g_Difficulty == GameDifficulty::NORMAL) DrawStringToHandle(280, 370, L">", normColor, difficultyFontHandle);
    if (g_Difficulty == GameDifficulty::HARD) DrawStringToHandle(300, 420, L">", hardColor, difficultyFontHandle);
    
    if ((timer / 30) % 2 == 0) {
        DrawStringToHandle(230, 500, L"PRESS ENTER TO START", GetColor(0, 255, 255), subFontHandle);
    }
}

void TitleScene::Finalize() {
    DeleteFontToHandle(titleFontHandle);
    DeleteFontToHandle(subFontHandle);
    DeleteFontToHandle(difficultyFontHandle);
    DeleteGraph(bgImageHandle);
    StopMusic();
}
