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
    keyWaitTimer = 0;
    
    if (g_Score > 0) {
        if (g_Ranking.size() < 5 || g_Score > g_Ranking.back().score) {
            isInputtingName = true;
            nameChars[0] = 'A';
            nameChars[1] = 'A';
            nameChars[2] = 'A';
            charIndex = 0;
        }
    }
    
    PlayMusic(L"C:\\Windows\\Media\\town.mid", DX_PLAYTYPE_LOOP);
}

SceneType RankingScene::Update() {
    if (isInputtingName) {
        if (keyWaitTimer > 0) keyWaitTimer--;
        
        if (keyWaitTimer == 0) {
            if (CheckHitKey(KEY_INPUT_UP)) {
                nameChars[charIndex]++;
                if (nameChars[charIndex] > 'Z') nameChars[charIndex] = 'A';
                keyWaitTimer = 10;
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            }
            if (CheckHitKey(KEY_INPUT_DOWN)) {
                nameChars[charIndex]--;
                if (nameChars[charIndex] < 'A') nameChars[charIndex] = 'Z';
                keyWaitTimer = 10;
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            }
            if (CheckHitKey(KEY_INPUT_LEFT)) {
                charIndex--;
                if (charIndex < 0) charIndex = 2;
                keyWaitTimer = 15;
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            }
            if (CheckHitKey(KEY_INPUT_RIGHT)) {
                charIndex++;
                if (charIndex > 2) charIndex = 0;
                keyWaitTimer = 15;
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            }
            if (CheckHitKey(KEY_INPUT_RETURN)) {
                std::string finalName = "";
                finalName += nameChars[0];
                finalName += nameChars[1];
                finalName += nameChars[2];
                SaveRanking(finalName, g_Score);
                g_Score = 0;
                isInputtingName = false;
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
        DrawStringToHandle(220, 180, L"ENTER YOUR IDENTIFICATION", GetColor(0, 255, 255), subFontHandle);
        
        for (int i = 0; i < 3; i++) {
            int bx = 280 + i * 90;
            int by = 280;
            
            if (i == charIndex) {
                int alpha = 150 + (int)(sin(GetNowCount() * 0.01f) * 105);
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
                DrawBox(bx - 15, by - 10, bx + 65, by + 75, GetColor(0, 255, 100), TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                
                DrawBox(bx - 15, by - 10, bx + 65, by + 75, GetColor(255, 255, 255), FALSE);
            }
            
            wchar_t cStr[2];
            cStr[0] = nameChars[i];
            cStr[1] = L'\0';
            DrawStringToHandle(bx, by, cStr, GetColor(255, 255, 255), inputFontHandle);
            
            if (i == charIndex) {
                DrawTriangle(bx + 25, by - 25, bx + 10, by - 40, bx + 40, by - 40, GetColor(0, 255, 255), TRUE);
                DrawTriangle(bx + 25, by + 100, bx + 10, by + 115, bx + 40, by + 115, GetColor(0, 255, 255), TRUE);
            }
        }
        
        DrawFormatStringToHandle(280, 420, GetColor(0, 255, 255), subFontHandle, L"SCORE: %06d", g_Score);
        
        int pulseText = 150 + (int)(sin(GetNowCount() * 0.01f) * 105);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, pulseText);
        DrawStringToHandle(120, 500, L"USE ARROW KEYS TO EDIT, ENTER TO SAVE", GetColor(150, 255, 150), subFontHandle);
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
            
            DrawFormatStringToHandle(180, y + 10, textColor, subFontHandle, L"RANK %d    %s", (int)i + 1, wn.c_str());
            DrawFormatStringToHandle(400, y + 10, GetColor(0, 255, 255), subFontHandle, L"%06d PTS", g_Ranking[i].score);
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
    DeleteFontToHandle(titleFontHandle);
    DeleteFontToHandle(subFontHandle);
    DeleteFontToHandle(inputFontHandle);
}
