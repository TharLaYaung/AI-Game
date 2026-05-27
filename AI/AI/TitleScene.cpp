#include "TitleScene.h"
#include "GameTypes.h"
#include "DxLib.h"
#include <math.h>

void TitleScene::Initialize() {
    timer = 0;
    menuIndex = 0;
    showSettings = false;
    inputCooldown = 0;
    
    introState = 0;
    introTimer = 0;
    
    titleFontHandle = CreateFontToHandle(L"Impact", 64, 5, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
    subFontHandle = CreateFontToHandle(L"Arial Black", 32, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
    difficultyFontHandle = CreateFontToHandle(L"Consolas", 28, 4, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
    bgImageHandle = LoadGraph(L"bg_cyberpunk.png");
    
    crawlScreenHandle = MakeScreen(800, 2000, TRUE);
    SetDrawScreen(crawlScreenHandle);
    ClearDrawScreen();
    int yellow = GetColor(255, 220, 50);
    
    int wTitle = GetDrawStringWidthToHandle(L"THE AI MENACE", 13, titleFontHandle);
    DrawStringToHandle(400 - (wTitle / 2), 150, L"THE AI MENACE", yellow, titleFontHandle);
    
    const wchar_t* lines[] = {
        L"It is a period of cyber war.",
        L"Rogue AI spaceships, striking",
        L"from a hidden mainframe, have",
        L"won their first victory against",
        L"the human galactic empire.",
        L"",
        L"During the battle, our brave",
        L"cyborg pilots managed to steal",
        L"secret plans to the AI's",
        L"ultimate weapon, the DEATH STAR",
        L"an armored space station with",
        L"enough power to destroy an",
        L"entire planet.",
        L"",
        L"Now, you must fly your fighter",
        L"into the heart of the machine",
        L"and restore freedom to the",
        L"galaxy..."
    };
    for (int i = 0; i < 18; i++) {
        DrawStringToHandle(50, 350 + i * 50, lines[i], yellow, subFontHandle);
    }
    SetDrawScreen(DX_SCREEN_BACK);
    
    PlayMusic(L"C:\\Windows\\Media\\flourish.mid", DX_PLAYTYPE_LOOP);
}

SceneType TitleScene::Update() {
    timer++;
    
    if (introState < 3) {
        introTimer++;
        if (CheckHitKey(KEY_INPUT_RETURN) || CheckHitKey(KEY_INPUT_SPACE) || CheckHitKey(KEY_INPUT_ESCAPE) || (GetMouseInput() & MOUSE_INPUT_LEFT)) {
            introState = 3;
            inputCooldown = 30;
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
        }
        
        if (introState == 0 && introTimer > 240) { introState = 1; introTimer = 0; }
        if (introState == 1 && introTimer > 180) { introState = 2; introTimer = 0; }
        if (introState == 2 && introTimer > 1200) { introState = 3; introTimer = 0; }
        
        return SceneType::TITLE;
    }
    
    if (inputCooldown > 0) inputCooldown--;

    if (inputCooldown == 0) {
        int mouseX, mouseY;
        GetMousePoint(&mouseX, &mouseY);
        
        for (int i = 0; i < 4; i++) {
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
            if (menuIndex < 0) menuIndex = 3;
            inputCooldown = 15;
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
        }
        else if (CheckHitKey(KEY_INPUT_DOWN) == 1 || CheckHitKey(KEY_INPUT_S) == 1) {
            menuIndex++;
            if (menuIndex > 3) menuIndex = 0;
            inputCooldown = 15;
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
        }
    }
    
    if (inputCooldown == 0 && (CheckHitKey(KEY_INPUT_RETURN) == 1 || (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)) {
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
        inputCooldown = 15;
        if (menuIndex == 0) return SceneType::CHARACTER_SELECT;
        if (menuIndex == 1) return SceneType::RANKING;
        if (menuIndex == 2) return SceneType::SETTINGS;
        if (menuIndex == 3) {
            g_ExitGame = true;
            return SceneType::TITLE;
        }
    }
    return SceneType::TITLE; 
}

void TitleScene::Draw() {
    if (introState == 0) {
        int alpha = 0;
        if (introTimer < 60) alpha = (introTimer * 255) / 60;
        else if (introTimer < 180) alpha = 255;
        else alpha = 255 - ((introTimer - 180) * 255) / 60;
        
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        const wchar_t* prologueText = L"A long time ago in a galaxy far, far away...";
        int textW = GetDrawStringWidthToHandle(prologueText, 44, difficultyFontHandle);
        DrawStringToHandle(400 - (textW / 2), 250, prologueText, GetColor(0, 255, 255), difficultyFontHandle);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        return;
    }
    
    DrawExtendGraph(0, 0, 800, 600, bgImageHandle, TRUE);
    
    int gridSize = 40;
    int offset = timer % gridSize;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
    for (int i = 0; i < 800; i += gridSize) {
        DrawLine(i, 0, i, 600, GetColor(0, 255, 255));
        DrawLine(0, i + offset, 800, i + offset, GetColor(0, 255, 255));
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    
    if (introState == 1) {
        float scale = 3.0f - (introTimer / 180.0f) * 2.8f;
        int alpha = 255;
        if (introTimer > 150) alpha = 255 - ((introTimer - 150) * 255) / 30;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        
        int w = GetDrawStringWidthToHandle(L"CYBORG BATTLE", 13, titleFontHandle);
        DrawRotaStringToHandle(400 - (int)(w * scale / 2), 300, scale, scale, 0.0, 0.0, 0.0, GetColor(0, 255, 255), titleFontHandle, GetColor(0,0,0), FALSE, L"CYBORG BATTLE");
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        return;
    }
    
    if (introState == 2) {
        float scrollY = introTimer * 1.0f;
        
        float h = 1500.0f; 
        DrawModiGraph(
            300, 600 - scrollY, 
            500, 600 - scrollY, 
            900, 600 + h - scrollY, 
            -100, 600 + h - scrollY, 
            crawlScreenHandle, TRUE
        );
        return;
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(80, 60, 720, 560, GetColor(0, 10, 30), TRUE);
    DrawBox(80, 60, 720, 560, GetColor(0, 255, 255), FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    int yOffset = (int)(sin(timer * 0.05f) * 10);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawStringToHandle(155, 105 + yOffset, L"CYBORG BATTLE", GetColor(255, 0, 255), titleFontHandle);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawStringToHandle(150, 100 + yOffset, L"CYBORG BATTLE", GetColor(0, 255, 255), titleFontHandle);
    
    const wchar_t* options[4] = { L"START GAME", L"RANKING", L"SETTINGS", L"EXIT" };
    for (int i = 0; i < 4; i++) {
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
    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
    DrawStringToHandle(250, 520, L"PRESS ESC TO SKIP INTRO NEXT TIME", GetColor(200, 200, 200), difficultyFontHandle);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::Finalize() {
    DeleteFontToHandle(titleFontHandle);
    DeleteFontToHandle(subFontHandle);
    DeleteFontToHandle(difficultyFontHandle);
    DeleteGraph(bgImageHandle);
    DeleteGraph(crawlScreenHandle);
}
