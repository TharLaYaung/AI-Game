#include "RankingScene.h"
#include "GameTypes.h"
#include "DxLib.h"
#include <math.h>

void RankingScene::Initialize() {
    timer = 0;
    titleFontHandle = CreateFontToHandle(L"Impact", 60, 5, DX_FONTTYPE_ANTIALIASING_EDGE);
    subFontHandle = CreateFontToHandle(L"Arial Black", 28, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
    inputFontHandle = CreateFontToHandle(L"Impact", 50, 5, DX_FONTTYPE_ANTIALIASING_EDGE);
    
    isInputtingName = false;
    keyWaitTimer = 30; // Wait before accepting input to prevent accidental skips
    
    if (g_Score > 0) {
        if (g_Ranking.size() < 5 || g_Score > g_Ranking.back().score) {
            isInputtingName = true;
            keyInputHandle = MakeKeyInput(10, FALSE, FALSE, FALSE);
            SetActiveKeyInput(keyInputHandle);
            SetKeyInputStringFont(inputFontHandle);
        }
    }
    
    PlayMusic(L"C:\\Windows\\Media\\town.mid", DX_PLAYTYPE_LOOP);
}

SceneType RankingScene::Update() {
    if (isInputtingName) {
        if (keyWaitTimer > 0) keyWaitTimer--;
        
        if (keyWaitTimer == 0) {
            if (CheckKeyInput(keyInputHandle) != 0) {
                TCHAR nameBuf[256];
                GetKeyInputString(nameBuf, keyInputHandle);
                
                int requiredSize = WideCharToMultiByte(CP_ACP, 0, nameBuf, -1, NULL, 0, NULL, NULL);
                std::string finalName(requiredSize - 1, '\0');
                WideCharToMultiByte(CP_ACP, 0, nameBuf, -1, &finalName[0], requiredSize, NULL, NULL);
                
                if (finalName.empty()) finalName = "UNKNOWN";
                
                SaveRanking(finalName, g_Score);
                g_Score = 0;
                isInputtingName = false;
                DeleteKeyInput(keyInputHandle);
                timer = 0; 
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
                keyWaitTimer = 30; 
            }
        }
        return SceneType::RANKING;
    }

    timer++;
    if (keyWaitTimer > 0) keyWaitTimer--;
    
    if (timer < 30 || keyWaitTimer > 0) return SceneType::RANKING;
    
    if (CheckHitKey(KEY_INPUT_RETURN) == 1 || (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
        return SceneType::TITLE;
    }
    return SceneType::RANKING;
}

void RankingScene::Draw() {
    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
    DrawBox(0, 0, 800, 600, GetColor(0, 10, 20), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 30);
    for (int y = 0; y < 600; y += 4) {
        DrawLine(0, y, 800, y, GetColor(0, 255, 100));
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    if (isInputtingName) {
        DrawStringToHandle(140, 100, L"NEW RECORD ESTABLISHED", GetColor(255, 200, 0), titleFontHandle);
        DrawStringToHandle(220, 180, L"TYPE YOUR IDENTIFICATION", GetColor(0, 255, 255), subFontHandle);
        
        int boxWidth = 350;
        int bx = 400 - boxWidth / 2;
        int by = 280;
        
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
        DrawBox(bx, by, bx + boxWidth, by + 75, GetColor(0, 150, 255), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        DrawBox(bx, by, bx + boxWidth, by + 75, GetColor(0, 255, 255), FALSE);
        
        DrawKeyInputString(bx + 20, by + 10, keyInputHandle);
        DrawKeyInputModeString(bx + 20, by + 80);
        
        int minutes = (g_ClearTime / 60) / 60;
        int seconds = (g_ClearTime / 60) % 60;
        int milliseconds = (int)(((g_ClearTime % 60) / 60.0f) * 1000.0f);
        DrawFormatStringToHandle(280, 420, GetColor(0, 255, 255), subFontHandle, L"SCORE: %06d  TIME: %02d:%02d.%03d", g_Score, minutes, seconds, milliseconds);
        
        int pulseText = 150 + (int)(sin(GetNowCount() * 0.01f) * 105);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, pulseText);
        DrawStringToHandle(180, 500, L"TYPE YOUR NAME, PRESS ENTER TO SAVE", GetColor(150, 255, 150), subFontHandle);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        
    } else {
        
        int titleAlpha = (timer * 10 > 255) ? 255 : timer * 10;
        int slideY = (timer < 30) ? (int)(-50 + sin(timer * 3.14f / 60.0f) * 50) : 0;
        
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, titleAlpha);
        DrawStringToHandle(120, 50 + slideY, L"=== TOP RANKING ===", GetColor(0, 255, 255), titleFontHandle);
        
        int pulse = 150 + (int)(sin(timer * 0.1f) * 105);
        SetDrawBlendMode(DX_BLENDMODE_ADD, pulse);
        DrawStringToHandle(120, 50 + slideY, L"=== TOP RANKING ===", GetColor(0, 100, 255), titleFontHandle);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        int startY = 150;
        for (size_t i = 0; i < g_Ranking.size(); i++) {
            if (i >= 5) break; 

            int boxAlpha = (timer > 20 + i * 10) ? 200 : 0;
            if (boxAlpha == 0) continue;

            int y = startY + (int)i * 65;
            
            int boxColor = GetColor(20, 40, 60);
            int textColor = GetColor(255, 255, 255);
            int frameColor = GetColor(0, 255, 255);
            
            if (i == 0) {
                boxColor = GetColor(60, 50, 0);
                textColor = GetColor(255, 215, 0); 
                frameColor = GetColor(255, 215, 0);
            } else if (i == 1) {
                boxColor = GetColor(40, 40, 40);
                textColor = GetColor(200, 200, 200); 
                frameColor = GetColor(200, 200, 200);
            } else if (i == 2) {
                boxColor = GetColor(50, 30, 10);
                textColor = GetColor(205, 127, 50); 
                frameColor = GetColor(205, 127, 50);
            }
            
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, boxAlpha);
            DrawBox(150, y, 650, y + 50, boxColor, TRUE);
            DrawBox(150, y, 650, y + 50, frameColor, FALSE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            
            std::string n = g_Ranking[i].name;
            std::wstring wn(n.begin(), n.end());
            
            DrawFormatStringToHandle(50, y + 10, textColor, subFontHandle, L"RANK %d", (int)i + 1);
            DrawStringToHandle(180, y + 10, wn.c_str(), textColor, subFontHandle);
            DrawFormatStringToHandle(410, y + 10, GetColor(0, 255, 255), subFontHandle, L"%06d PTS", g_Ranking[i].score);
            
            int rMins = (g_Ranking[i].clearTime / 60) / 60;
            int rSecs = (g_Ranking[i].clearTime / 60) % 60;
            int rMs = (int)(((g_Ranking[i].clearTime % 60) / 60.0f) * 1000.0f);
            if (g_Ranking[i].clearTime < 99999) {
                DrawFormatStringToHandle(620, y + 10, GetColor(255, 255, 0), subFontHandle, L"%02d:%02d.%03d", rMins, rSecs, rMs);
            } else {
                DrawStringToHandle(620, y + 10, L"--:--.---", GetColor(100, 100, 100), subFontHandle);
            }
        }
        
        if (g_Ranking.empty() && timer > 30) {
            DrawStringToHandle(250, 250, L"NO RECORDS YET", GetColor(150, 150, 150), subFontHandle);
        }
        
        if (timer > 60) {
            int alpha = 150 + (int)(sin(timer * 0.1f) * 100);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
            DrawStringToHandle(220, 500, L"PRESS ENTER TO RETURN", GetColor(255, 100, 100), subFontHandle);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }
}

void RankingScene::Finalize() {
    if (isInputtingName) {
        DeleteKeyInput(keyInputHandle);
    }
    DeleteFontToHandle(titleFontHandle);
    DeleteFontToHandle(subFontHandle);
    DeleteFontToHandle(inputFontHandle);
}
