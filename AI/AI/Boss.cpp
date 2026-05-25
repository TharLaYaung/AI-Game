#include "Boss.h"
#include "GameTypes.h"
#include <math.h>

Boss::Boss() {}

void Boss::Initialize() {
    pos = VGet(0.0f, 150.0f, 300.0f);
    targetPos = pos;
    radius = 50.0f;
    
    if (g_Difficulty == GameDifficulty::EASY) hp = 300;
    else if (g_Difficulty == GameDifficulty::NORMAL) hp = 500;
    else if (g_Difficulty == GameDifficulty::HARD) hp = 1000;
    state = BossState::IDLE;
    stateTimer = 0;
    laserStrikeTimer = 0;
    laserStrikeWarningTimer = 0;
    laserStrikePos = VGet(0,0,0);
    isSecondForm = false;
    isFinalForm = false;
    bullets.clear();
}

void Boss::Update(VECTOR playerPos) {
    if (hp <= 0) {
        
        pos.y -= 3.0f;
        
        
        for (auto& b : bullets) {
            if (b.active) {
                b.pos = VAdd(b.pos, VScale(b.dir, b.speed));
                if (b.pos.z < -800.0f || b.pos.z > 800.0f || b.pos.x < -800.0f || b.pos.x > 800.0f || b.pos.y < -600.0f || b.pos.y > 600.0f) {
                    b.active = false;
                }
            }
        }
        return;
    }

    
    if (hp <= GetMaxHP() / 2 && !isSecondForm) {
        isSecondForm = true;
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Notify System Generic.wav", DX_PLAYTYPE_BACK); 
    }

    if (laserStrikeTimer > 0) {
        laserStrikeTimer--;
    } else if (laserStrikeWarningTimer > 0) {
        laserStrikeWarningTimer--;
        if (laserStrikeWarningTimer == 0) {
            laserStrikeTimer = 60; 
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Insert.wav", DX_PLAYTYPE_BACK);
        }
    } else {
        
        int baseLaserChance = (g_Difficulty == GameDifficulty::HARD) ? 200 : (g_Difficulty == GameDifficulty::EASY) ? 600 : 400;
        int secondLaserChance = (g_Difficulty == GameDifficulty::HARD) ? 80 : (g_Difficulty == GameDifficulty::EASY) ? 250 : 150;
        int finalLaserChance = (g_Difficulty == GameDifficulty::HARD) ? 30 : (g_Difficulty == GameDifficulty::EASY) ? 100 : 60;
        
        int laserChance = baseLaserChance;
        if (isFinalForm) laserChance = finalLaserChance; 
        else if (isSecondForm) laserChance = secondLaserChance;
        
        if (GetRand(laserChance) == 0) {
            laserStrikeWarningTimer = (g_Difficulty == GameDifficulty::HARD) ? 30 : (g_Difficulty == GameDifficulty::EASY) ? 90 : 60; 
            laserStrikePos = playerPos; 
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Default.wav", DX_PLAYTYPE_BACK);
        }
        
        if (isSecondForm || isFinalForm) {
            int fbChance = (g_Difficulty == GameDifficulty::HARD) ? 150 : (g_Difficulty == GameDifficulty::EASY) ? 800 : 400;
            if (isFinalForm) fbChance /= 2;
            
            if (GetRand(fbChance) == 0) {
                int bombWarning = (g_Difficulty == GameDifficulty::HARD) ? 45 : (g_Difficulty == GameDifficulty::EASY) ? 90 : 60;
                float bombRadius = (g_Difficulty == GameDifficulty::HARD) ? 150.0f : (g_Difficulty == GameDifficulty::EASY) ? 60.0f : 100.0f;
                int bombDuration = (g_Difficulty == GameDifficulty::HARD) ? 300 : (g_Difficulty == GameDifficulty::EASY) ? 120 : 180;
                
                FireBomb fb;
                fb.pos = VGet(playerPos.x + GetRand(200) - 100, playerPos.y + GetRand(200) - 100, playerPos.z + GetRand(100) - 50);
                fb.warningTimer = bombWarning;
                fb.activeTimer = bombDuration;
                fb.radius = bombRadius;
                fb.active = true;
                fireBombs.push_back(fb);
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Insert.wav", DX_PLAYTYPE_BACK);
            }
        }
    }
    
    for (auto& fb : fireBombs) {
        if (!fb.active) continue;
        if (fb.warningTimer > 0) fb.warningTimer--;
        else if (fb.activeTimer > 0) fb.activeTimer--;
        else fb.active = false;
    }
    
    stateTimer++;
    if (isSecondForm) stateTimer++; 
    if (isFinalForm) stateTimer += 2; 

    
    float moveSpeed = 0.03f;
    if (isFinalForm) moveSpeed = 0.15f; 
    else if (isSecondForm) moveSpeed = 0.05f;
    
    pos = VAdd(pos, VScale(VSub(targetPos, pos), moveSpeed));

    switch (state) {
        case BossState::IDLE:
            if (stateTimer > 60) {
                int attackProb = (g_Difficulty == GameDifficulty::HARD) ? 40 : (g_Difficulty == GameDifficulty::EASY) ? 70 : 50;
                if (GetRand(100) < attackProb) {
                    state = BossState::ATTACKING;
                } else {
                    state = BossState::MOVING;
                    targetPos.x = (float)(GetRand(1000) - 500);
                    targetPos.y = (float)(GetRand(400) - 100);
                    targetPos.z = (float)(GetRand(400) + 100);
                }
                stateTimer = 0;
            }
            break;
            
        case BossState::ATTACKING:
            if (stateTimer == 1) {
                VECTOR dir = VSub(playerPos, pos);
                dir = VNorm(dir);
                
                Bullet b;
                b.pos = pos;
                b.dir = dir;
                
                
                if (g_Difficulty == GameDifficulty::EASY) b.speed = 3.0f;
                else if (g_Difficulty == GameDifficulty::NORMAL) b.speed = 4.0f;
                else if (g_Difficulty == GameDifficulty::HARD) b.speed = 6.0f;
                
                b.radius = 10.0f;
                b.active = true;
                b.isReflected = false;
                bullets.push_back(b);
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Balloon.wav", DX_PLAYTYPE_BACK);
            }
            if (stateTimer > ((g_Difficulty == GameDifficulty::HARD) ? 15 : 30)) {
                state = BossState::IDLE;
                stateTimer = 0;
            }
            break;
            
        case BossState::MOVING:
            if (stateTimer > 60) {
                state = BossState::IDLE;
                stateTimer = 0;
            }
            break;
    }

    for (auto& b : bullets) {
        if (b.active) {
            
            if (!b.isReflected) {
                VECTOR toPlayer = VSub(playerPos, b.pos);
                toPlayer = VNorm(toPlayer);
                b.dir = VAdd(b.dir, VScale(toPlayer, 0.02f)); 
                b.dir = VNorm(b.dir);
            }

            b.pos = VAdd(b.pos, VScale(b.dir, b.speed));
            if (b.pos.z < -800.0f || b.pos.z > 800.0f || b.pos.x < -800.0f || b.pos.x > 800.0f || b.pos.y < -600.0f || b.pos.y > 600.0f) {
                b.active = false;
            }
        }
    }
}

void Boss::Draw() {
    
        
        
        int hullColor = GetColor(180, 180, 200);
        if (isFinalForm) hullColor = GetColor(10, 10, 10); 
        else if (isSecondForm) hullColor = GetColor(150, 20, 20);
        
        VECTOR top = VAdd(pos, VGet(0, radius * 0.8f, 0));
        VECTOR bottom = VAdd(pos, VGet(0, -radius * 0.5f, 0));
        DrawCapsule3D(top, bottom, radius, 32, hullColor, GetColor(255, 255, 255), TRUE);
        
        
        float armorAngle = stateTimer * 0.05f;
        int plateColor = GetColor(200, 200, 220);
        if (isFinalForm) plateColor = GetColor(30, 30, 30); 
        else if (isSecondForm) plateColor = GetColor(100, 10, 10);
        
        for (int i = 0; i < 4; i++) {
            float a = armorAngle + (3.14159f / 2.0f) * i;
            VECTOR platePos = VAdd(pos, VGet(cos(a) * radius * 1.4f, 0, sin(a) * radius * 1.4f));
            DrawSphere3D(platePos, radius * 0.4f, 16, plateColor, GetColor(255,255,255), TRUE);
            
            VECTOR spikeEnd = VAdd(platePos, VGet(cos(a) * radius * 2.0f, 0, sin(a) * radius * 2.0f));
            int spikeColor = isFinalForm ? GetColor(0, 255, 255) : GetColor(150, 0, 0);
            DrawCone3D(platePos, spikeEnd, radius * 0.2f, 8, spikeColor, GetColor(255, 255, 255), TRUE);
        }
        
        
        int eyeColor = isFinalForm ? GetColor(0, 255, 255) : GetColor(255, 0, 0); 
        VECTOR eyePos = VAdd(pos, VGet(0, 10.0f, -radius * 0.95f));
        SetDrawBlendMode(DX_BLENDMODE_ADD, 200);
        DrawSphere3D(eyePos, radius * 0.4f, 16, eyeColor, GetColor(255, 255, 255), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        DrawSphere3D(eyePos, radius * 0.2f, 16, GetColor(255, 200, 200), GetColor(255, 255, 255), TRUE);
        
        
        float energyAngle = -stateTimer * 0.1f;
        VECTOR part1 = VAdd(pos, VGet(cos(energyAngle) * radius * 1.5f, -radius * 0.8f, sin(energyAngle) * radius * 1.5f));
        VECTOR part2 = VAdd(pos, VGet(cos(energyAngle + 3.14f) * radius * 1.5f, -radius * 0.8f, sin(energyAngle + 3.14f) * radius * 1.5f));
        SetDrawBlendMode(DX_BLENDMODE_ADD, 150);
        DrawSphere3D(part1, radius * 0.2f, 8, GetColor(0, 255, 255), GetColor(255,255,255), TRUE);
        DrawSphere3D(part2, radius * 0.2f, 8, GetColor(0, 255, 255), GetColor(255,255,255), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    for (const auto& b : bullets) {
        if (b.active) {
            int r = b.isReflected ? 0 : 255;
            int g = b.isReflected ? 255 : 50;
            int b_col = b.isReflected ? 255 : 0;
            
            SetDrawBlendMode(DX_BLENDMODE_ADD, 200);
            DrawSphere3D(b.pos, b.radius, 16, GetColor(r, g, b_col), GetColor(255, 255, 255), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
            VECTOR endPos = VAdd(b.pos, VScale(b.dir, 20.0f));
            DrawCapsule3D(b.pos, endPos, b.radius, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
        }
    }
    
    for (const auto& fb : fireBombs) {
        if (!fb.active) continue;
        if (fb.warningTimer > 0) {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
            DrawCylinder3D(VGet(fb.pos.x, -5.0f, fb.pos.z), VGet(fb.pos.x, 5.0f, fb.pos.z), fb.radius, 16, GetColor(255, 100, 0), GetColor(255, 100, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        } else if (fb.activeTimer > 0) {
            int alpha = 150 + GetRand(100);
            SetDrawBlendMode(DX_BLENDMODE_ADD, alpha);
            DrawSphere3D(VGet(fb.pos.x, 0.0f, fb.pos.z), fb.radius, 16, GetColor(255, 50, 0), GetColor(255, 255, 255), TRUE);
            DrawSphere3D(VGet(fb.pos.x, 0.0f, fb.pos.z), fb.radius * 0.7f, 16, GetColor(255, 255, 0), GetColor(255, 255, 255), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }
}
