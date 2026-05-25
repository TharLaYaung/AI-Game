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
    currentState = MainState::PLAYING;
    midBuffIndex = 1; 
    midBuffCooldown = 0;
    buffSelectTimer = 0;
    
    cardLaserImg = LoadGraph(L"img\\card_laser.png");
    cardBombImg = LoadGraph(L"img\\card_bomb.png");
    cardSpeedImg = LoadGraph(L"img\\card_speed.png");
    
    camPos = VGet(0, 300, -500);

    SetCameraNearFar(10.0f, 2000.0f);
    SetCameraPositionAndTarget_UpVecY(camPos, VGet(0, 0, 0));

    SetGlobalAmbientLight(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
    
    hudFontHandle = CreateFontToHandle(L"Arial Black", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
    smallFontHandle = CreateFontToHandle(L"Arial Black", 16, 2, DX_FONTTYPE_ANTIALIASING_EDGE);
    
    miniEnemies.clear();
    items.clear();
    
    stars.clear();
    for (int i = 0; i < 300; i++) {
        Star s;
        s.pos = VGet((float)(GetRand(4000) - 2000), (float)(GetRand(2000) - 1000), (float)(GetRand(4000) - 1000));
        s.speed = 2.0f + (float)GetRand(20);
        int brightness = 150 + GetRand(105);
        s.color = GetColor(brightness, brightness, brightness);
        stars.push_back(s);
    }
    
    g_Score = 0;
    isPaused = false;
    
    SetMouseDispFlag(FALSE);
    
    PlayMusic(L"C:\\Windows\\Media\\town.mid", DX_PLAYTYPE_LOOP);
}

SceneType MainScene::Update() {
    static bool prevPauseKey = false;
    bool currPauseKey = CheckHitKey(KEY_INPUT_P);
    if (currPauseKey && !prevPauseKey) {
        isPaused = !isPaused;
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Ding.wav", DX_PLAYTYPE_BACK);
    }
    prevPauseKey = currPauseKey;

    if (isPaused) {
        if (CheckHitKey(KEY_INPUT_RETURN)) {
            return SceneType::TITLE;
        }
        return SceneType::MAIN;
    }

    if (currentState == MainState::MID_BUFF_SELECT) {
        buffSelectTimer++;
        if (midBuffCooldown > 0) midBuffCooldown--;

        if (midBuffCooldown == 0) {
            int mouseX, mouseY;
            GetMousePoint(&mouseX, &mouseY);
            
            for (int i = 0; i < 3; i++) {
                int bx = 120 + i * 200;
                int by = 220; 
                if (mouseX >= bx && mouseX <= bx + 150 && mouseY >= by && mouseY <= by + 200) {
                    if (midBuffIndex != i) {
                        midBuffIndex = i;
                        PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
                    }
                    if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
                        g_Buff = (midBuffIndex == 0) ? BuffType::LASER : (midBuffIndex == 1) ? BuffType::BOMB : BuffType::SPEED;
                        currentState = MainState::PLAYING;
                        SetMouseDispFlag(FALSE);
                        PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
                        midBuffCooldown = 30;
                    }
                }
            }
            
            if (CheckHitKey(KEY_INPUT_LEFT) || CheckHitKey(KEY_INPUT_A)) {
                midBuffIndex--;
                if (midBuffIndex < 0) midBuffIndex = 2;
                midBuffCooldown = 15;
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            }
            else if (CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_D)) {
                midBuffIndex++;
                if (midBuffIndex > 2) midBuffIndex = 0;
                midBuffCooldown = 15;
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            }
            
            if (CheckHitKey(KEY_INPUT_RETURN)) {
                g_Buff = (midBuffIndex == 0) ? BuffType::LASER : (midBuffIndex == 1) ? BuffType::BOMB : BuffType::SPEED;
                currentState = MainState::PLAYING;
                SetMouseDispFlag(FALSE);
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
                midBuffCooldown = 30;
            }
        }
        return SceneType::MAIN;
    }

    if (boss->GetHP() <= 0) {
        if (!boss->IsFinalForm()) {
            boss->EnterFinalForm();
            effectManager->AddExplosion(boss->GetPos(), GetColor(0, 255, 255), 200.0f);
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
            PlaySoundFile(L"C:\\Windows\\Media\\Ring08.wav", DX_PLAYTYPE_LOOP); 
            
            currentState = MainState::MID_BUFF_SELECT;
            SetMouseDispFlag(TRUE);
            midBuffIndex = 0;
            midBuffCooldown = 30;
            return SceneType::MAIN;
        } else {
            if (clearTimer == 0) g_Score += 10000;
            
            
            if (clearTimer % 4 == 0) {
                VECTOR bossPos = boss->GetPos();
                VECTOR offset = VGet((float)(GetRand(300) - 150), (float)(GetRand(300) - 150), (float)(GetRand(300) - 150));
                effectManager->AddExplosion(VAdd(bossPos, offset), GetColor(255, 150 + GetRand(105), 0), 40.0f + GetRand(60));
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Critical Stop.wav", DX_PLAYTYPE_BACK);
            }
            
            clearTimer++;
            if (clearTimer > 180) return SceneType::RESULT; 
        }
    }
    if (player->GetHP() <= 0) {
        clearTimer++;
        if (clearTimer > 60) return SceneType::GAMEOVER;
    }

    VECTOR pPos = player->GetPos();
    VECTOR targetCamPos = VGet(pPos.x, pPos.y + 300.0f, pPos.z - 500.0f);
    camPos = VAdd(camPos, VScale(VSub(targetCamPos, camPos), 0.1f)); 
    SetCameraPositionAndTarget_UpVecY(camPos, pPos);

    player->Update();
    boss->Update(player->GetPos());
    effectManager->Update();
    
    if (player->IsFiringLaser()) {
        VECTOR pp = player->GetPos();
        VECTOR bp = boss->GetPos();
        if (bp.z > pp.z) {
            float distXY = (bp.x - pp.x) * (bp.x - pp.x) + (bp.y - pp.y) * (bp.y - pp.y);
            if (distXY < (30.0f + boss->GetRadius()) * (30.0f + boss->GetRadius())) {
                boss->Damage(2); 
                g_Score += 2;
                if (GetRand(3) == 0) effectManager->AddExplosion(VGet(bp.x, bp.y, pp.z + (bp.z - pp.z) * 0.5f), GetColor(0, 255, 255), 5);
            }
        }
        
        for (auto& e : miniEnemies) {
            if (!e.active) continue;
            if (e.pos.z > pp.z) {
                float distXY = (e.pos.x - pp.x) * (e.pos.x - pp.x) + (e.pos.y - pp.y) * (e.pos.y - pp.y);
                if (distXY < 40.0f * 40.0f) { 
                    e.hp -= 3;
                    if (GetRand(3) == 0) effectManager->AddExplosion(e.pos, GetColor(0, 255, 255), 5);
                }
            }
        }
    }

    for (auto& b : player->GetBullets()) {
        if (!b.active) continue;
        
        
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
        if (VSquareSize(diff) < (b.radius + boss->GetRadius() + 80.0f) * (b.radius + boss->GetRadius() + 80.0f)) {
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
                if (player->GetBarrierGauge() >= 15.0f) {
                    player->ConsumeBarrier(15.0f);
                    b.isReflected = true;
                    b.dir = VScale(b.dir, -1.0f);
                    b.speed *= 2.0f;
                    b.pos = VAdd(b.pos, VScale(b.dir, b.speed));
                    effectManager->AddExplosion(b.pos, GetColor(0, 255, 255), 15);
                    PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
                } else {
                    
                    player->ForceAttackForm();
                    player->ConsumeBarrier(100.0f);
                    player->Damage(20);
                    b.active = false;
                    effectManager->AddExplosion(player->GetPos(), GetColor(255, 0, 0), 20);
                    PlaySoundFile(L"C:\\Windows\\Media\\Windows Critical Stop.wav", DX_PLAYTYPE_BACK);
                    
                    camPos = VGet(pPos.x + GetRand(40)-20, pPos.y + 300.0f + GetRand(40)-20, pPos.z - 500.0f);
                    SetCameraPositionAndTarget_UpVecY(camPos, pPos);
                }
            } else {
                  player->Damage(20);
                  b.active = false;
                  effectManager->AddExplosion(player->GetPos(), GetColor(255, 0, 0), 20);
                  PlaySoundFile(L"C:\\Windows\\Media\\Windows Critical Stop.wav", DX_PLAYTYPE_BACK);
                  
                  camPos = VGet(pPos.x + GetRand(40)-20, pPos.y + 300.0f + GetRand(40)-20, pPos.z - 500.0f);
                  SetCameraPositionAndTarget_UpVecY(camPos, pPos);
              }
        }
        
        if (b.isReflected) {
            VECTOR diffBoss = VSub(b.pos, boss->GetPos());
            if (VSquareSize(diffBoss) < (b.radius + boss->GetRadius() + 80.0f) * (b.radius + boss->GetRadius() + 80.0f)) {
                boss->Damage(50);
                g_Score += 50;
                b.active = false;
                effectManager->AddExplosion(b.pos, GetColor(0, 255, 255), 30);
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Critical Stop.wav", DX_PLAYTYPE_BACK);
            }
        }
    }

    
    if (boss->GetLaserStrikeTimer() > 0) {
        VECTOR lp = boss->GetLaserStrikePos();
        
        
        if (boss->GetLaserStrikeTimer() % 4 == 0) {
            effectManager->AddExplosion(VGet(lp.x + GetRand(80)-40, 0.0f, lp.z + GetRand(80)-40), GetColor(255, 100, 50), 30);
        }
        
        
        VECTOR pp = player->GetPos();
        camPos = VGet(pp.x + GetRand(30)-15, pp.y + 300.0f + GetRand(30)-15, pp.z - 500.0f + GetRand(30)-15);
        SetCameraPositionAndTarget_UpVecY(camPos, pp);
        
        float distSq = (lp.x - pp.x) * (lp.x - pp.x) + (lp.z - pp.z) * (lp.z - pp.z);
        float laserRadius = (g_Difficulty == GameDifficulty::HARD) ? 150.0f : (g_Difficulty == GameDifficulty::EASY) ? 70.0f : 100.0f;
        if (distSq < (laserRadius + player->GetRadius()) * (laserRadius + player->GetRadius())) {
            
            if (player->GetForm() == PlayerForm::BARRIER) {
                if (player->GetBarrierGauge() > 0) {
                    float drain = (g_Difficulty == GameDifficulty::HARD) ? 5.0f : (g_Difficulty == GameDifficulty::EASY) ? 1.0f : 2.0f;
                    player->ConsumeBarrier(drain); 
                } else {
                    player->ForceAttackForm();
                    int dmg = (g_Difficulty == GameDifficulty::HARD) ? 2 : 1;
                    player->Damage(dmg); 
                }
            } else {
                int dmg = (g_Difficulty == GameDifficulty::HARD) ? 5 : (g_Difficulty == GameDifficulty::EASY) ? 1 : 2;
                player->Damage(dmg); 
            }
        }
        
        for (const auto& fb : boss->GetFireBombs()) {
            if (!fb.active || fb.warningTimer > 0) continue;
            float distSqFB = (fb.pos.x - pp.x) * (fb.pos.x - pp.x) + (fb.pos.z - pp.z) * (fb.pos.z - pp.z);
            if (distSqFB < (fb.radius + player->GetRadius()) * (fb.radius + player->GetRadius())) {
                if (player->GetShieldTimer() == 0) {
                    int fbDmg = (g_Difficulty == GameDifficulty::HARD) ? 4 : (g_Difficulty == GameDifficulty::EASY) ? 1 : 2;
                    player->Damage(fbDmg);
                }
            }
        }
    }

    
    if (GetRand(100) < 3) {
        MiniEnemy e;
        e.pos = VGet((float)(GetRand(1000) - 500), (float)(250 + GetRand(200)), 1000.0f);
        e.speed = 3.0f + GetRand(4);
        e.hp = 10;
        e.active = true;
        miniEnemies.push_back(e);
    }
    
    
    for (auto& e : miniEnemies) {
        if (!e.active) continue;
        e.pos.z -= e.speed;
        if (e.pos.z < -1000.0f) e.active = false;
    }
    
    
    int itemChance = (g_Difficulty == GameDifficulty::HARD) ? 2 : (g_Difficulty == GameDifficulty::EASY) ? 10 : 5;
    if (GetRand(1000) < itemChance) {
        Item newItem;
        newItem.pos = VGet(player->GetPos().x + (GetRand(800) - 400), player->GetPos().y + (GetRand(400) - 200), player->GetPos().z + 800.0f);
        int r = GetRand(10);
        if (r > 7) newItem.type = BuffType::LASER_WEAPON;
        else if (r > 3) newItem.type = BuffType::HEAL;
        else newItem.type = BuffType::SHIELD;
        newItem.active = true;
        newItem.timer = 900; 
        items.push_back(newItem);
    }

    for (auto& it : items) {
        if (!it.active) continue;
        
        it.timer--;
        if (it.timer <= 0) {
            it.active = false;
            continue;
        }
        
        it.pos.z -= 1.0f;
        
        VECTOR diff = VSub(player->GetPos(), it.pos);
        if (VSquareSize(diff) < 2500.0f) {
            if (it.type == BuffType::HEAL) {
                player->Damage(-30); 
                g_Score += 200;
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            } else if (it.type == BuffType::SHIELD) {
                player->AddShield(600); 
                g_Score += 200;
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            } else if (it.type == BuffType::LASER_WEAPON) {
                player->AddLaserWeapon(900); // 15 seconds
                g_Score += 200;
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            } else {
                g_Buff = it.type;
                g_Score += 500;
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Navigation Start.wav", DX_PLAYTYPE_BACK);
            }
            it.active = false;
        }
    }

    
    for (auto& s : stars) {
        s.pos.z -= s.speed;
        if (s.pos.z < -1000.0f) {
            s.pos.z = 3000.0f;
            s.pos.x = (float)(GetRand(4000) - 2000);
            s.pos.y = (float)(GetRand(2000) - 1000);
        }
    }

    return SceneType::MAIN;
}

void MainScene::Draw() {
    
    for (const auto& s : stars) {
        VECTOR endPos = VAdd(s.pos, VGet(0, 0, s.speed * 2.0f));
        DrawLine3D(s.pos, endPos, s.color);
    }

    
    VECTOR pPos = player->GetPos();
    int basex = ((int)pPos.x / 200) * 200;
    int basez = ((int)pPos.z / 200) * 200;
    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
    for (int i = -1600; i <= 1600; i += 200) {
        
        DrawLine3D(VGet(basex + (float)i, -300.0f, basez - 1600.0f), VGet(basex + (float)i, -300.0f, basez + 1600.0f), GetColor(0, 255, 100));
        DrawLine3D(VGet(basex - 1600.0f, -300.0f, basez + (float)i), VGet(basex + 1600.0f, -300.0f, basez + (float)i), GetColor(0, 255, 100));
        
        DrawLine3D(VGet(basex + (float)i, 700.0f, basez - 1600.0f), VGet(basex + (float)i, 700.0f, basez + 1600.0f), GetColor(0, 150, 255));
        DrawLine3D(VGet(basex - 1600.0f, 700.0f, basez + (float)i), VGet(basex + 1600.0f, 700.0f, basez + (float)i), GetColor(0, 150, 255));
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    player->Draw();
    boss->Draw();
    effectManager->Draw();

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(0, 0, 800, 40, GetColor(0, 20, 50), TRUE);
    DrawBox(0, 40, 800, 42, GetColor(0, 255, 255), TRUE);
    
    for (const auto& e : miniEnemies) {
        if (!e.active) continue;
        DrawCapsule3D(VAdd(e.pos, VGet(-15,0,0)), VAdd(e.pos, VGet(15,0,0)), 15.0f, 8, GetColor(255, 50, 50), GetColor(255, 255, 255), TRUE);
    }
    
    
    for (const auto& it : items) {
        if (!it.active) continue;
        
        if (it.type == BuffType::HEAL) {
            SetDrawBlendMode(DX_BLENDMODE_ADD, 150 + (int)(sin(GetNowCount() * 0.01f) * 100));
            VECTOR p1 = VAdd(it.pos, VGet(-8, -20, -8));
            VECTOR p2 = VAdd(it.pos, VGet(8, 20, 8));
            DrawCube3D(p1, p2, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);
            VECTOR p3 = VAdd(it.pos, VGet(-20, -8, -8));
            VECTOR p4 = VAdd(it.pos, VGet(20, 8, 8));
            DrawCube3D(p3, p4, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        } else if (it.type == BuffType::SHIELD) {
            SetDrawBlendMode(DX_BLENDMODE_ADD, 150 + (int)(sin(GetNowCount() * 0.01f) * 100));
            DrawSphere3D(it.pos, 15.0f, 16, GetColor(0, 150, 255), GetColor(255, 255, 255), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        } else {
            int color = GetColor(255, 255, 255);
            if (it.type == BuffType::SHIELD) {
                color = GetColor(0, 150, 255);
            } else if (it.type == BuffType::LASER_WEAPON) {
                color = GetColor(0, 255, 255);
            } else if (it.type == BuffType::LASER) color = GetColor(0, 255, 255);
            else if (it.type == BuffType::BOMB) color = GetColor(255, 100, 0);
            else if (it.type == BuffType::SPEED) color = GetColor(255, 255, 0);
            
            VECTOR p1 = VAdd(it.pos, VGet(-10, -10, -10));
            VECTOR p2 = VAdd(it.pos, VGet(10, 10, 10));
            DrawCube3D(p1, p2, color, GetColor(255, 255, 255), TRUE);
        }
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    
    
    
    static int uiFrame = 0;
    uiFrame++;
    
    
    int pulse = 180 + (int)(sin(uiFrame * 0.1f) * 75);

    
    int chX = 400, chY = 300;
    int chRot = uiFrame * 2;
    int chSize = 15 + (int)(sin(uiFrame * 0.2f) * 3);
    DrawLine(chX - chSize, chY, chX + chSize, chY, GetColor(0, 255, 255));
    DrawLine(chX, chY - chSize, chX, chY + chSize, GetColor(0, 255, 255));
    DrawCircle(chX, chY, chSize - 5, GetColor(0, 200, 255), FALSE);
    
    
    static int displayScore = g_Score;
    static int lastScore = g_Score;
    static int scorePopTimer = 0;
    
    
    if (g_Score > lastScore) {
        scorePopTimer = 15; 
        lastScore = g_Score;
    }
    
    
    if (displayScore < g_Score) {
        displayScore += (g_Score - displayScore) / 4 + 1;
        if (displayScore > g_Score) displayScore = g_Score;
    }
    if (scorePopTimer > 0) scorePopTimer--;

    int boxAlpha = (scorePopTimer > 0) ? 255 : 200;
    int borderColor = (scorePopTimer > 0) ? GetColor(255, 255, 255) : GetColor(0, 255, 255);
    int textColor = (scorePopTimer > 0) ? GetColor(255, 255, 255) : GetColor(255, 215, 0);
    
    
    int shakeX = (scorePopTimer > 0) ? (GetRand(4) - 2) : 0;
    int shakeY = (scorePopTimer > 0) ? (GetRand(4) - 2) : 0;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, boxAlpha);
    DrawBox(10 + shakeX, 10 + shakeY, 200 + shakeX, 45 + shakeY, GetColor(0, 20, 50), TRUE);
    DrawBox(10 + shakeX, 10 + shakeY, 200 + shakeX, 45 + shakeY, borderColor, FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    
    DrawFormatStringToHandle(20 + shakeX, 18 + shakeY, textColor, hudFontHandle, L"SCORE: %06d", displayScore);

    
    int bossMaxHp = boss->GetMaxHP();
    float hpRatio = (float)boss->GetHP() / (float)bossMaxHp;
    if (hpRatio < 0.0f) hpRatio = 0.0f;
    
    DrawStringToHandle(280, 15, L"T A R G E T   I N T E G R I T Y", GetColor(255, 100, 100), smallFontHandle);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(280, 35, 520, 45, GetColor(50, 0, 0), TRUE);
    DrawBox(280, 35, 280 + (int)(hpRatio * 240.0f), 45, GetColor(255, 50, 50), TRUE);
    DrawBox(278, 33, 522, 47, GetColor(255, 100, 100), FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    
    int blX = 10;
    int blY = 485;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
    DrawBox(blX, blY, blX + 330, blY + 105, GetColor(0, 30, 30), TRUE);
    DrawBox(blX, blY, blX + 330, blY + 105, GetColor(0, 255, 200), FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    
    DrawStringToHandle(blX + 10, blY + 5, L"SYSTEM INTEGRITY", GetColor(0, 255, 255), hudFontHandle);
    
    float pRatio = (float)player->GetHP() / 100.0f; 
    if (pRatio < 0.0f) pRatio = 0.0f;
    DrawStringToHandle(blX + 10, blY + 45, L"HP", GetColor(255, 100, 100), smallFontHandle);
    DrawBox(blX + 80, blY + 45, blX + 310, blY + 55, GetColor(50, 20, 20), TRUE);
    DrawBox(blX + 80, blY + 45, blX + 80 + (int)(pRatio * 230.0f), blY + 55, GetColor(255, 50, 50), TRUE);
    
    float barrierRatio = player->GetBarrierGauge() / player->GetMaxBarrierGauge();
    DrawStringToHandle(blX + 10, blY + 65, L"BARRIER", GetColor(200, 100, 255), smallFontHandle);
    DrawBox(blX + 80, blY + 65, blX + 310, blY + 75, GetColor(20, 20, 50), TRUE);
    DrawBox(blX + 80, blY + 65, blX + 80 + (int)(barrierRatio * 230.0f), blY + 75, GetColor(200, 50, 255), TRUE);
    
    if (player->HasLaserWeapon() || player->IsOverheated()) {
        float heatRatio = player->GetLaserHeat() / 100.0f;
        int heatColor = player->IsOverheated() ? GetColor(255, 0, 0) : GetColor(255, 100, 0);
        DrawStringToHandle(blX + 10, blY + 85, player->IsOverheated() ? L"OVERHEAT" : L"LASER", heatColor, smallFontHandle);
        DrawBox(blX + 80, blY + 85, blX + 310, blY + 95, GetColor(50, 0, 0), TRUE);
        DrawBox(blX + 80, blY + 85, blX + 80 + (int)(heatRatio * 230.0f), blY + 95, heatColor, TRUE);
    }

    
    int brX = 470;
    int brY = 510;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
    DrawBox(brX, brY, brX + 320, brY + 80, GetColor(30, 20, 0), TRUE);
    DrawBox(brX, brY, brX + 320, brY + 80, GetColor(255, 200, 0), FALSE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    if (player->GetForm() == PlayerForm::ATTACK) {
        DrawStringToHandle(brX + 10, brY + 15, L"MODE: [ATTACK]", GetColor(255, 100, 100), hudFontHandle);
    } else {
        DrawStringToHandle(brX + 10, brY + 15, L"MODE: [BARRIER]", GetColor(100, 200, 255), hudFontHandle);
    }
    
    const wchar_t* buffName = L"NONE";
    if (g_Buff == BuffType::LASER) buffName = L"PIERCING LASER";
    else if (g_Buff == BuffType::BOMB) buffName = L"EXPLOSIVE ROUNDS";
    else if (g_Buff == BuffType::SPEED) buffName = L"SPEED BOOST";
    
    DrawStringToHandle(brX + 10, brY + 50, L"BUFF:", GetColor(255, 200, 0), smallFontHandle);
    DrawStringToHandle(brX + 70, brY + 45, buffName, GetColor(255, 255, 255), hudFontHandle);

    
    int warn = boss->GetLaserStrikeWarningTimer();
    int active = boss->GetLaserStrikeTimer();
    VECTOR lpos = boss->GetLaserStrikePos();
    
    float laserRadius = (g_Difficulty == GameDifficulty::HARD) ? 150.0f : (g_Difficulty == GameDifficulty::EASY) ? 70.0f : 100.0f;
    
    if (warn > 0) {
        
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
        DrawCylinder3D(VGet(lpos.x, -500.0f, lpos.z), VGet(lpos.x, 1000.0f, lpos.z), laserRadius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        
        
        if ((warn / 5) % 2 == 0) {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
            DrawBox(0, 250, 800, 350, GetColor(255, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            DrawStringToHandle(140, 285, L"CAUTION: ORBITAL LASER STRIKE", GetColor(255, 255, 0), hudFontHandle);
        }
    } else if (active > 0) {
        
        int rJitter = GetRand(20) - 10;
        int alpha = 200 + GetRand(55);
        
        SetDrawBlendMode(DX_BLENDMODE_ADD, alpha);
        DrawCylinder3D(VGet(lpos.x, -500.0f, lpos.z), VGet(lpos.x, 1000.0f, lpos.z), laserRadius + 20.0f + rJitter, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
        
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawCylinder3D(VGet(lpos.x, -500.0f, lpos.z), VGet(lpos.x, 1000.0f, lpos.z), (laserRadius * 0.7f) + rJitter * 0.5f, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
        
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    
    if (isPaused) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(0, 0, 800, 600, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        
        int pauseFont = CreateFontToHandle(L"Arial Black", 40, 5, DX_FONTTYPE_ANTIALIASING_EDGE);
        DrawStringToHandle(300, 200, L"PAUSED", GetColor(0, 255, 255), pauseFont);
        DrawStringToHandle(260, 300, L"Press P to Resume", GetColor(255, 255, 255), hudFontHandle);
        DrawStringToHandle(220, 350, L"Press Enter to Return Title", GetColor(255, 100, 100), hudFontHandle);
        DeleteFontToHandle(pauseFont);
    }

    if (currentState == MainState::MID_BUFF_SELECT) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
        DrawBox(0, 0, 800, 600, GetColor(0, 10, 20), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        
        int headerFont = CreateFontToHandle(L"Impact", 50, 5, DX_FONTTYPE_ANTIALIASING_EDGE);
        
        
        int slideY = (buffSelectTimer < 30) ? (int)(-100 + sin(buffSelectTimer * 3.14f / 60.0f) * 100) : 0;
        
        int pulseText = 150 + (int)(sin(GetNowCount() * 0.01f) * 105);
        SetDrawBlendMode(DX_BLENDMODE_ADD, pulseText);
        DrawStringToHandle(120, 80 + slideY, L"SYSTEM UPGRADE AVAILABLE", GetColor(0, 255, 255), headerFont);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        
        DrawStringToHandle(280, 160 + slideY, L"SELECT NEW GEAR", GetColor(255, 255, 0), hudFontHandle);

        const wchar_t* options[3] = { L"LASER", L"BOMB", L"SPEED" };
        int images[3] = { cardLaserImg, cardBombImg, cardSpeedImg };
        int colors[3] = { GetColor(255, 0, 255), GetColor(255, 150, 0), GetColor(0, 255, 100) };
        
        
        for (int i = 0; i < 3; i++) {
            int bx = 120 + i * 200;
            int by = 220 + slideY;
            
            
            float scale = (buffSelectTimer < 40) ? (buffSelectTimer / 40.0f) : 1.0f;
            if (scale > 1.0f) scale = 1.0f;
            
            int drawBx = bx + (int)(150 * (1.0f - scale) / 2.0f);
            int drawBy = by + (int)(200 * (1.0f - scale) / 2.0f);
            
            if (midBuffIndex == i) {
                
                SetDrawBlendMode(DX_BLENDMODE_ADD, pulseText);
                DrawBox(drawBx - 5, drawBy - 5, drawBx + (int)(150 * scale) + 5, drawBy + (int)(200 * scale) + 5, colors[i], TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                DrawExtendGraph(drawBx, drawBy, drawBx + (int)(150 * scale), drawBy + (int)(200 * scale), images[i], TRUE);
                
                SetDrawBlendMode(DX_BLENDMODE_ADD, pulseText);
                DrawStringToHandle(bx + 40, by + 210, options[i], colors[i], hudFontHandle);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            } else {
                
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
                DrawExtendGraph(drawBx, drawBy, drawBx + (int)(150 * scale), drawBy + (int)(200 * scale), images[i], TRUE);
                DrawStringToHandle(bx + 40, by + 210, options[i], GetColor(100, 100, 100), hudFontHandle);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            }
        }
        
        DrawStringToHandle(200, 480 + slideY, L"PRESS ENTER TO EQUIP AND RESUME", GetColor(255, 255, 255), hudFontHandle);
        DeleteFontToHandle(headerFont);
    }
}

void MainScene::Finalize() {
    DeleteFontToHandle(hudFontHandle);
    DeleteFontToHandle(smallFontHandle);
    DeleteGraph(cardLaserImg);
    DeleteGraph(cardBombImg);
    DeleteGraph(cardSpeedImg);
    StopMusic();
    
    delete player;
    delete boss;
    delete effectManager;
}
