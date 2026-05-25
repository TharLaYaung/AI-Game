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
            if (mouseX >= 100 && mouseX <= 300 && mouseY >= y && mouseY <= y + 40) {
                GameDifficulty diff = (i == 0) ? GameDifficulty::EASY : (i == 1) ? GameDifficulty::NORMAL : GameDifficulty::HARD;
                if (g_Difficulty != diff) {
                    g_Difficulty = diff;
                    PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
                }
            }
        }
        
        for (int i = 0; i < 4; i++) {
            int y = 240 + i * 50;
            if (mouseX >= 450 && mouseX <= 700 && mouseY >= y && mouseY <= y + 40) {
                BuffType buff = (i == 0) ? BuffType::NONE : (i == 1) ? BuffType::LASER : (i == 2) ? BuffType::BOMB : BuffType::SPEED;
                if (g_Buff != buff) {
                    g_Buff = buff;
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
            if (g_Buff == BuffType::LASER) g_Buff = BuffType::NONE;
            else if (g_Buff == BuffType::BOMB) g_Buff = BuffType::LASER;
            else if (g_Buff == BuffType::SPEED) g_Buff = BuffType::BOMB;
            else if (g_Buff == BuffType::NONE) g_Buff = BuffType::SPEED;
            inputCooldown = 12;
        }
        else if (CheckHitKey(KEY_INPUT_RIGHT) == 1 || CheckHitKey(KEY_INPUT_D) == 1) {
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            if (g_Buff == BuffType::NONE) g_Buff = BuffType::LASER;
            else if (g_Buff == BuffType::LASER) g_Buff = BuffType::BOMB;
            else if (g_Buff == BuffType::BOMB) g_Buff = BuffType::SPEED;
            else if (g_Buff == BuffType::SPEED) g_Buff = BuffType::NONE;
            inputCooldown = 12;
        }
    }
    
    if (CheckHitKey(KEY_INPUT_RETURN) == 1 || (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
        return SceneType::MAIN;
    }
    return SceneType::CHARACTER_SELECT;
}

void CharacterSelectScene::Draw() {
    DrawExtendGraph(0, 0, 800, 600, bgImageHandle, TRUE);
    
    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(50, 40, 750, 560, GetColor(10, 30, 0), TRUE);
    DrawBox(50, 40, 750, 560, GetColor(0, 255, 100), FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    int yOffset = (int)(sin(timer * 0.05f) * 10);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawStringToHandle(105, 65 + yOffset, L"MISSION DEPLOYMENT", GetColor(0, 255, 0), mainFontHandle);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawStringToHandle(100, 60 + yOffset, L"MISSION DEPLOYMENT", GetColor(150, 255, 150), mainFontHandle);

    
    DrawStringToHandle(70, 180, L"DIFFICULTY (UP/DOWN)", GetColor(0, 255, 255), subFontHandle);
    
    int easyColor = (g_Difficulty == GameDifficulty::EASY) ? GetColor(0, 255, 0) : GetColor(100, 100, 100);
    int normColor = (g_Difficulty == GameDifficulty::NORMAL) ? GetColor(255, 255, 0) : GetColor(100, 100, 100);
    int hardColor = (g_Difficulty == GameDifficulty::HARD) ? GetColor(255, 0, 0) : GetColor(100, 100, 100);
    
    DrawStringToHandle(120, 240, L"EASY", easyColor, subFontHandle);
    DrawStringToHandle(120, 290, L"NORMAL", normColor, subFontHandle);
    DrawStringToHandle(120, 340, L"HARD", hardColor, subFontHandle);
    
    if (g_Difficulty == GameDifficulty::EASY) DrawStringToHandle(90, 240, L">", easyColor, subFontHandle);
    if (g_Difficulty == GameDifficulty::NORMAL) DrawStringToHandle(90, 290, L">", normColor, subFontHandle);
    if (g_Difficulty == GameDifficulty::HARD) DrawStringToHandle(90, 340, L">", hardColor, subFontHandle);

    
    DrawStringToHandle(420, 180, L"GEAR (LEFT/RIGHT)", GetColor(0, 255, 255), subFontHandle);
    
    int noneColor = (g_Buff == BuffType::NONE) ? GetColor(255, 255, 255) : GetColor(100, 100, 100);
    int laserColor = (g_Buff == BuffType::LASER) ? GetColor(255, 0, 255) : GetColor(100, 100, 100);
    int bombColor = (g_Buff == BuffType::BOMB) ? GetColor(255, 150, 0) : GetColor(100, 100, 100);
    int speedColor = (g_Buff == BuffType::SPEED) ? GetColor(0, 255, 100) : GetColor(100, 100, 100);
    
    DrawStringToHandle(470, 240, L"STANDARD", noneColor, subFontHandle);
    DrawStringToHandle(470, 290, L"LASER", laserColor, subFontHandle);
    DrawStringToHandle(470, 340, L"BOMB", bombColor, subFontHandle);
    DrawStringToHandle(470, 390, L"SPEED", speedColor, subFontHandle);
    
    if (g_Buff == BuffType::NONE) DrawStringToHandle(440, 240, L">", noneColor, subFontHandle);
    if (g_Buff == BuffType::LASER) DrawStringToHandle(440, 290, L">", laserColor, subFontHandle);
    if (g_Buff == BuffType::BOMB) DrawStringToHandle(440, 340, L">", bombColor, subFontHandle);
    if (g_Buff == BuffType::SPEED) DrawStringToHandle(440, 390, L">", speedColor, subFontHandle);

    
    if ((timer / 30) % 2 == 0) {
        DrawStringToHandle(260, 480, L"PRESS ENTER TO DEPLOY", GetColor(255, 255, 0), difficultyFontHandle);
    }
}

void CharacterSelectScene::Finalize() {
    DeleteFontToHandle(mainFontHandle);
    DeleteFontToHandle(subFontHandle);
    DeleteFontToHandle(difficultyFontHandle);
    DeleteGraph(bgImageHandle);
}
