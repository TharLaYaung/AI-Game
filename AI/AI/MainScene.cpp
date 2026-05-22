#include "MainScene.h"
#include "GameTypes.h"
#include "DxLib.h"
#include <cmath>

void MainScene::Initialize() {
    player = new Player();
    player->Initialize();

    boss = new Boss();
    boss->Initialize();

    effectManager = new EffectManager();
    effectManager->Initialize();

    clearTimer = 0;

    SetCameraNearFar(10.0f, 2000.0f);
    SetCameraPositionAndTarget_UpVecY(VGet(0, 300, -500), VGet(0, 0, 0));

    SetGlobalAmbientLight(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
    
    bgImageHandle = LoadGraph(L"bg_cyberpunk.png");
    hudFontHandle = CreateFontToHandle(L"Impact", 24, 2, DX_FONTTYPE_ANTIALIASING_EDGE);
    smallFontHandle = CreateFontToHandle(L"Consolas", 16, 2, DX_FONTTYPE_ANTIALIASING_EDGE);
    
    PlayMusic(L"C:\\Windows\\Media\\town.mid", DX_PLAYTYPE_LOOP);
}

SceneType MainScene::Update() {
    if (boss->GetHP() <= 0) {
        clearTimer++;
        if (clearTimer > 60) return SceneType::RESULT;
    }
    if (player->GetHP() <= 0) {
        clearTimer++;
        if (clearTimer > 60) return SceneType::GAMEOVER;
    }

    VECTOR pPos = player->GetPos();
    SetCameraPositionAndTarget_UpVecY(VGet(pPos.x, 300, pPos.z - 500), pPos);

    player->Update();
    boss->Update(player->GetPos());
    effectManager->Update();

    for (auto& b : player->GetBullets()) {
        if (!b.active) continue;
        VECTOR diff = VSub(b.pos, boss->GetPos());
        if (VSquareSize(diff) < (b.radius + boss->GetRadius()) * (b.radius + boss->GetRadius())) {
            boss->Damage(10);
            b.active = false;
            effectManager->AddExplosion(b.pos, GetColor(255, 255, 0), 10);
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Critical Stop.wav", DX_PLAYTYPE_BACK);
        }
    }

    for (auto& b : boss->GetBullets()) {
        if (!b.active) continue;
        VECTOR diff = VSub(b.pos, player->GetPos());
        if (VSquareSize(diff) < (b.radius + player->GetRadius()) * (b.radius + player->GetRadius())) {
            if (player->GetForm() == PlayerForm::BARRIER) {
                b.isReflected = true;
                b.dir = VScale(b.dir, -1.0f);
                b.speed *= 2.0f;
                b.pos = VAdd(b.pos, VScale(b.dir, b.speed));
                effectManager->AddExplosion(b.pos, GetColor(0, 255, 255), 15);
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
            } else {
                player->Damage(20);
                b.active = false;
                effectManager->AddExplosion(player->GetPos(), GetColor(255, 0, 0), 20);
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Critical Stop.wav", DX_PLAYTYPE_BACK);
                
                SetCameraPositionAndTarget_UpVecY(
                    VGet(pPos.x + GetRand(20)-10, 300 + GetRand(20)-10, pPos.z - 500), 
                    pPos);
            }
        }
        
        if (b.isReflected) {
            VECTOR diffBoss = VSub(b.pos, boss->GetPos());
            if (VSquareSize(diffBoss) < (b.radius + boss->GetRadius()) * (b.radius + boss->GetRadius())) {
                boss->Damage(50);
                b.active = false;
                effectManager->AddExplosion(b.pos, GetColor(0, 255, 255), 30);
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Critical Stop.wav", DX_PLAYTYPE_BACK);
            }
        }
    }

    return SceneType::MAIN;
}

void MainScene::Draw() {
    DrawExtendGraph(0, 0, 800, 600, bgImageHandle, TRUE);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
    for (int i = -1000; i <= 1000; i += 100) {
        DrawLine3D(VGet((float)i, 0.0f, -1000.0f), VGet((float)i, 0.0f, 1000.0f), GetColor(0, 150, 255));
        DrawLine3D(VGet(-1000.0f, 0.0f, (float)i), VGet(1000.0f, 0.0f, (float)i), GetColor(0, 150, 255));
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    player->Draw();
    boss->Draw();
    effectManager->Draw();

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(0, 0, 800, 40, GetColor(0, 20, 50), TRUE);
    DrawBox(0, 40, 800, 42, GetColor(0, 255, 255), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    DrawStringToHandle(10, 10, L"// CYBER_COMBAT_PROTOCOL_ENGAGED", GetColor(0, 255, 255), smallFontHandle);
    DrawStringToHandle(400, 10, L"MOVE: WASD | FIRE: Space | FORM: Shift", GetColor(200, 200, 255), smallFontHandle);

    DrawLine(390, 300, 395, 300, GetColor(0, 255, 255));
    DrawLine(405, 300, 410, 300, GetColor(0, 255, 255));
    DrawLine(400, 290, 400, 295, GetColor(0, 255, 255));
    DrawLine(400, 305, 400, 310, GetColor(0, 255, 255));
    DrawCircle(400, 300, 15, GetColor(0, 255, 255), FALSE);

    DrawStringToHandle(10, 50, L"SYSTEM INTEGRITY (PLAYER)", GetColor(0, 255, 255), hudFontHandle);
    DrawBox(10, 80, 210, 95, GetColor(50, 50, 50), TRUE);
    DrawBox(10, 80, 10 + player->GetHP() * 2, 95, GetColor(0, 255, 150), TRUE);
    DrawBox(10, 80, 210, 95, GetColor(0, 255, 255), FALSE);

    DrawStringToHandle(500, 50, L"TARGET INTEGRITY (BOSS)", GetColor(255, 100, 100), hudFontHandle);
    DrawBox(580, 80, 780, 95, GetColor(50, 50, 50), TRUE);
    int bossMaxHp = (g_Difficulty == GameDifficulty::EASY) ? 300 : (g_Difficulty == GameDifficulty::HARD) ? 1000 : 500;
    DrawBox(580, 80, 580 + (int)(boss->GetHP() * 200.0f / bossMaxHp), 95, GetColor(255, 50, 50), TRUE);
    DrawBox(580, 80, 780, 95, GetColor(255, 100, 100), FALSE);

    int yPos = 550;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(10, yPos, 400, yPos + 35, GetColor(0, 20, 50), TRUE);
    DrawBox(10, yPos, 400, yPos + 35, GetColor(0, 255, 255), FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    if (player->GetForm() == PlayerForm::ATTACK) {
        DrawStringToHandle(20, yPos + 5, L"MODE: [ATTACK] - WEAPONS ACTIVE", GetColor(255, 100, 100), hudFontHandle);
    } else {
        DrawStringToHandle(20, yPos + 5, L"MODE: [BARRIER] - SHIELD ACTIVE", GetColor(100, 200, 255), hudFontHandle);
    }
}

void MainScene::Finalize() {
    DeleteGraph(bgImageHandle);
    DeleteFontToHandle(hudFontHandle);
    DeleteFontToHandle(smallFontHandle);
    StopMusic();
    
    delete player;
    delete boss;
    delete effectManager;
}
