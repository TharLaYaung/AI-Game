#include "CharacterSelectScene.h"
#include "GameTypes.h"
#include "DxLib.h"
#include <math.h>

void CharacterSelectScene::Initialize() {
    timer = 0;
    mainFontHandle = CreateFontToHandle(L"Impact", 50, 5, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
    subFontHandle = CreateFontToHandle(L"Consolas", 32, 4, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
    difficultyFontHandle = CreateFontToHandle(L"Arial Black", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
    bgImageHandle = LoadGraph(L"bg_cyberpunk.png");
    selectedBuff = BuffType::NONE;
}

SceneType CharacterSelectScene::Update() {
    timer++;
    if (timer < 30) return SceneType::CHARACTER_SELECT;
    
    static int inputCooldown = 0;
    if (inputCooldown > 0) inputCooldown--;

    if (inputCooldown == 0) {
        int mouseX, mouseY;
        GetMousePoint(&mouseX, &mouseY);
        
        for (int i = 0; i < 3; i++) {
            int y = 240 + i * 50;
            if (mouseX >= 80 && mouseX <= 300 && mouseY >= y && mouseY <= y + 40) {
                GameDifficulty diff = (i == 0) ? GameDifficulty::EASY : (i == 1) ? GameDifficulty::NORMAL : GameDifficulty::HARD;
                if (g_Difficulty != diff) {
                    g_Difficulty = diff;
                    PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
                }
            }
        }
        
        for (int i = 0; i < 4; i++) {
            int y = 240 + i * 50;
            if (mouseX >= 480 && mouseX <= 700 && mouseY >= y && mouseY <= y + 40) {
                BuffType buff = (i == 0) ? BuffType::NONE : (i == 1) ? BuffType::LASER : (i == 2) ? BuffType::BOMB : BuffType::SPEED;
                if (selectedBuff != buff) {
                    selectedBuff = buff;
                    PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
                }
            }
        }

        
        if (CheckHitKey(KEY_INPUT_UP) == 1 || CheckHitKey(KEY_INPUT_W) == 1) {
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            if (g_Difficulty == GameDifficulty::NORMAL) g_Difficulty = GameDifficulty::EASY;
            else if (g_Difficulty == GameDifficulty::HARD) g_Difficulty = GameDifficulty::NORMAL;
            else if (g_Difficulty == GameDifficulty::EASY) g_Difficulty = GameDifficulty::HARD;
            inputCooldown = 12;
        }
        else if (CheckHitKey(KEY_INPUT_DOWN) == 1 || CheckHitKey(KEY_INPUT_S) == 1) {
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            if (g_Difficulty == GameDifficulty::EASY) g_Difficulty = GameDifficulty::NORMAL;
            else if (g_Difficulty == GameDifficulty::NORMAL) g_Difficulty = GameDifficulty::HARD;
            else if (g_Difficulty == GameDifficulty::HARD) g_Difficulty = GameDifficulty::EASY;
            inputCooldown = 12;
        }
        
        
        if (CheckHitKey(KEY_INPUT_LEFT) == 1 || CheckHitKey(KEY_INPUT_A) == 1) {
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            if (selectedBuff == BuffType::LASER) selectedBuff = BuffType::NONE;
            else if (selectedBuff == BuffType::BOMB) selectedBuff = BuffType::LASER;
            else if (selectedBuff == BuffType::SPEED) selectedBuff = BuffType::BOMB;
            else if (selectedBuff == BuffType::NONE) selectedBuff = BuffType::SPEED;
            inputCooldown = 12;
        }
        else if (CheckHitKey(KEY_INPUT_RIGHT) == 1 || CheckHitKey(KEY_INPUT_D) == 1) {
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            if (selectedBuff == BuffType::NONE) selectedBuff = BuffType::LASER;
            else if (selectedBuff == BuffType::LASER) selectedBuff = BuffType::BOMB;
            else if (selectedBuff == BuffType::BOMB) selectedBuff = BuffType::SPEED;
            else if (selectedBuff == BuffType::SPEED) selectedBuff = BuffType::NONE;
            inputCooldown = 12;
        }
    }
    
    if (CheckHitKey(KEY_INPUT_RETURN) == 1 || (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
        g_Buffs.clear();
        if (selectedBuff != BuffType::NONE) {
            g_Buffs[selectedBuff] = 1;
        }
        g_CurrentStage = 1;
        return SceneType::MAIN; 
    }
    return SceneType::CHARACTER_SELECT;
}

void CharacterSelectScene::Draw() {
    DrawExtendGraph(0, 0, 800, 600, bgImageHandle, TRUE);
    
    int t = timer * 2; 

    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(40, 40, 760, 560, GetColor(0, 20, 10), TRUE);
    DrawBox(40, 40, 760, 560, GetColor(0, 255, 100), FALSE);
    
    
    for(int i=0; i<10; i++) {
        int y = 40 + ((t + i*52) % 520);
        DrawLine(40, y, 760, y, GetColor(0, 50, 20));
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    
    int yOffset = (int)(sin(timer * 0.05f) * 10);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawStringToHandle(105, 65 + yOffset, L"MISSION DEPLOYMENT", GetColor(0, 255, 0), mainFontHandle);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawStringToHandle(100, 60 + yOffset, L"MISSION DEPLOYMENT", GetColor(150, 255, 150), mainFontHandle);

    
    int leftX = 60;
    int rightX = 460;

    
    DrawStringToHandle(leftX, 180, L"DIFFICULTY (UP/DOWN)", GetColor(0, 255, 255), subFontHandle);
    DrawStringToHandle(rightX, 180, L"GEAR (LEFT/RIGHT)", GetColor(0, 255, 255), subFontHandle);
    
    
    int boxAlpha = 100 + (int)(sin(timer * 0.2f) * 50);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, boxAlpha);
    DrawBox(leftX + 20, 240 + (int)g_Difficulty * 50, leftX + 200, 280 + (int)g_Difficulty * 50, GetColor(0, 150, 255), TRUE);
    
    int buffIndex = (selectedBuff == BuffType::NONE) ? 0 : (selectedBuff == BuffType::LASER) ? 1 : (selectedBuff == BuffType::BOMB) ? 2 : 3;
    DrawBox(rightX + 20, 240 + buffIndex * 50, rightX + 220, 280 + buffIndex * 50, GetColor(0, 150, 255), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    
    int diffColors[3] = { GetColor(100, 100, 100), GetColor(100, 100, 100), GetColor(100, 100, 100) };
    diffColors[(int)g_Difficulty] = (g_Difficulty == GameDifficulty::EASY) ? GetColor(0, 255, 0) : 
                                    (g_Difficulty == GameDifficulty::NORMAL) ? GetColor(255, 255, 0) : GetColor(255, 0, 0);
    
    DrawStringToHandle(leftX + 40, 245, L"EASY", diffColors[0], subFontHandle);
    DrawStringToHandle(leftX + 40, 295, L"NORMAL", diffColors[1], subFontHandle);
    DrawStringToHandle(leftX + 40, 345, L"HARD", diffColors[2], subFontHandle);
    
    DrawStringToHandle(leftX + 10, 245 + (int)g_Difficulty * 50, L">", diffColors[(int)g_Difficulty], subFontHandle);

    
    int gearColors[4] = { GetColor(100, 100, 100), GetColor(100, 100, 100), GetColor(100, 100, 100), GetColor(100, 100, 100) };
    gearColors[buffIndex] = (selectedBuff == BuffType::NONE) ? GetColor(255, 255, 255) :
                              (selectedBuff == BuffType::LASER) ? GetColor(255, 0, 255) :
                              (selectedBuff == BuffType::BOMB) ? GetColor(255, 150, 0) : GetColor(0, 255, 100);
                              
    DrawStringToHandle(rightX + 40, 245, L"STANDARD", gearColors[0], subFontHandle);
    DrawStringToHandle(rightX + 40, 295, L"LASER", gearColors[1], subFontHandle);
    DrawStringToHandle(rightX + 40, 345, L"BOMB", gearColors[2], subFontHandle);
    DrawStringToHandle(rightX + 40, 395, L"SPEED", gearColors[3], subFontHandle);
    
    DrawStringToHandle(rightX + 10, 245 + buffIndex * 50, L">", gearColors[buffIndex], subFontHandle);

    
    if ((timer / 30) % 2 == 0) {
        DrawStringToHandle(260, 480, L"PRESS ENTER TO DEPLOY", GetColor(255, 255, 0), difficultyFontHandle);
    } else {
        DrawStringToHandle(260, 480, L"PRESS ENTER TO DEPLOY", GetColor(100, 100, 0), difficultyFontHandle);
    }
}

void CharacterSelectScene::Finalize() {
    DeleteFontToHandle(mainFontHandle);
    DeleteFontToHandle(subFontHandle);
    DeleteFontToHandle(difficultyFontHandle);
    DeleteGraph(bgImageHandle);
}
