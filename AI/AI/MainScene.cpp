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
    hudFontHandle = CreateFontToHandle(L"Arial Black", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
    smallFontHandle = CreateFontToHandle(L"Arial Black", 16, 2, DX_FONTTYPE_ANTIALIASING_EDGE);
    
    miniEnemies.clear();
    items.clear();
    g_Score = 0;
    
    PlayMusic(L"C:\\Windows\\Media\\town.mid", DX_PLAYTYPE_LOOP);
}

SceneType MainScene::Update() {
    if (boss->GetHP() <= 0) {
        if (clearTimer == 0) g_Score += 10000;
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
        
        // Mini enemy collision
        bool hitMini = false;
        for (auto& e : miniEnemies) {
            if (!e.active) continue;
            VECTOR diff = VSub(e.pos, b.pos);
            if (VSquareSize(diff) < 1500.0f) {
                e.hp -= (g_Buff == BuffType::BOMB) ? 20 : 10;
                if (g_Buff != BuffType::LASER) b.active = false;
                effectManager->AddExplosion(b.pos, GetColor(255, 255, 0), (g_Buff == BuffType::BOMB) ? 30 : 10);
                
                if (e.hp <= 0) {
                    e.active = false;
                    g_Score += 100;
                    effectManager->AddExplosion(e.pos, GetColor(255, 100, 0), 40);
                    PlaySoundFile(L"C:\\Windows\\Media\\Windows Critical Stop.wav", DX_PLAYTYPE_BACK);
                    
                    if (GetRand(100) < 40) {
                        Item it;
                        it.pos = e.pos;
                        int r = GetRand(2);
                        if (r == 0) it.type = BuffType::LASER;
                        else if (r == 1) it.type = BuffType::BOMB;
                        else it.type = BuffType::SPEED;
                        it.active = true;
                        items.push_back(it);
                    }
                }
                hitMini = true;
                break;
            }
        }
        if (hitMini) continue;

        VECTOR diff = VSub(boss->GetPos(), b.pos);
        if (VSquareSize(diff) < (b.radius + boss->GetRadius()) * (b.radius + boss->GetRadius())) {
            boss->Damage((g_Buff == BuffType::BOMB) ? 20 : 10);
            g_Score += (g_Buff == BuffType::BOMB) ? 20 : 10;
            if (g_Buff != BuffType::LASER) b.active = false;
            effectManager->AddExplosion(b.pos, GetColor(255, 255, 0), (g_Buff == BuffType::BOMB) ? 30 : 10);
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
                g_Score += 50;
                b.active = false;
                effectManager->AddExplosion(b.pos, GetColor(0, 255, 255), 30);
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Critical Stop.wav", DX_PLAYTYPE_BACK);
            }
        }
    }

    // Spawn mini enemies
    if (GetRand(100) < 3) {
        MiniEnemy e;
        e.pos = VGet((float)(GetRand(1000) - 500), (float)(250 + GetRand(200)), 1000.0f);
        e.speed = 3.0f + GetRand(4);
        e.hp = 10;
        e.active = true;
        miniEnemies.push_back(e);
    }
    
    // Update mini enemies
    for (auto& e : miniEnemies) {
        if (!e.active) continue;
        e.pos.z -= e.speed;
        if (e.pos.z < -1000.0f) e.active = false;
    }
    
    // Update items
    for (auto& it : items) {
        if (!it.active) continue;
        it.pos.z -= 2.0f;
        if (it.pos.z < -1000.0f) it.active = false;
        
        VECTOR diff = VSub(player->GetPos(), it.pos);
        if (VSquareSize(diff) < 2500.0f) {
            g_Buff = it.type;
            g_Score += 500;
            it.active = false;
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
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
    // Draw mini enemies
    for (const auto& e : miniEnemies) {
        if (!e.active) continue;
        DrawCapsule3D(VAdd(e.pos, VGet(-15,0,0)), VAdd(e.pos, VGet(15,0,0)), 15.0f, 8, GetColor(255, 50, 50), GetColor(255, 255, 255), TRUE);
    }
    
    // Draw items
    for (const auto& it : items) {
        if (!it.active) continue;
        int color = GetColor(255, 255, 255);
        if (it.type == BuffType::LASER) color = GetColor(0, 255, 255);
        else if (it.type == BuffType::BOMB) color = GetColor(255, 100, 0);
        else if (it.type == BuffType::SPEED) color = GetColor(255, 255, 0);
        
        VECTOR p1 = VAdd(it.pos, VGet(-10, -10, -10));
        VECTOR p2 = VAdd(it.pos, VGet(10, 10, 10));
        DrawCube3D(p1, p2, color, GetColor(255, 255, 255), TRUE);
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    DrawStringToHandle(10, 10, L"// CYBER_COMBAT_PROTOCOL_ENGAGED", GetColor(0, 255, 255), smallFontHandle);
    DrawStringToHandle(10, 30, L"MOVE: WASD | FIRE: Space | FORM: Shift", GetColor(200, 200, 255), smallFontHandle);

    // Center Crosshair
    DrawLine(390, 300, 395, 300, GetColor(0, 255, 255));
    DrawLine(405, 300, 410, 300, GetColor(0, 255, 255));
    DrawLine(400, 290, 400, 295, GetColor(0, 255, 255));
    DrawLine(400, 305, 400, 310, GetColor(0, 255, 255));
    DrawCircle(400, 300, 15, GetColor(0, 255, 255), FALSE);

    // Player HP
    DrawStringToHandle(10, 60, L"SYSTEM INTEGRITY", GetColor(0, 255, 255), hudFontHandle);
    DrawBox(10, 90, 310, 105, GetColor(50, 50, 50), TRUE);
    DrawBox(10, 90, 10 + (int)(player->GetHP() * 3), 105, GetColor(0, 255, 150), TRUE);
    DrawBox(10, 90, 310, 105, GetColor(0, 255, 255), FALSE);
    
    DrawFormatStringToHandle(10, 115, GetColor(255, 215, 0), hudFontHandle, L"SCORE: %06d", g_Score);

    // Boss HP
    DrawStringToHandle(480, 60, L"TARGET INTEGRITY", GetColor(255, 100, 100), hudFontHandle);
    DrawBox(480, 90, 780, 105, GetColor(50, 50, 50), TRUE);
    int bossMaxHp = (g_Difficulty == GameDifficulty::EASY) ? 300 : (g_Difficulty == GameDifficulty::HARD) ? 1000 : 500;
    DrawBox(480, 90, 480 + (int)(boss->GetHP() * 300.0f / bossMaxHp), 105, GetColor(255, 50, 50), TRUE);
    DrawBox(480, 90, 780, 105, GetColor(255, 100, 100), FALSE);

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
    
    // Draw Current Buff
    const wchar_t* buffName = L"NONE";
    if (g_Buff == BuffType::LASER) buffName = L"PIERCING LASER";
    else if (g_Buff == BuffType::BOMB) buffName = L"EXPLOSIVE ROUNDS";
    else if (g_Buff == BuffType::SPEED) buffName = L"SPEED BOOST";
    DrawStringToHandle(450, yPos + 5, L"BUFF:", GetColor(255, 255, 0), hudFontHandle);
    DrawStringToHandle(530, yPos + 5, buffName, GetColor(255, 255, 255), hudFontHandle);
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
