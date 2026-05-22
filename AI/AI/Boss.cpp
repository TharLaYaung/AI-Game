#include "Boss.h"
#include "GameTypes.h"
#include <math.h>

Boss::Boss() {}

void Boss::Initialize() {
    pos = VGet(0.0f, 0.0f, 300.0f);
    radius = 50.0f;
    
    if (g_Difficulty == GameDifficulty::EASY) hp = 300;
    else if (g_Difficulty == GameDifficulty::NORMAL) hp = 500;
    else if (g_Difficulty == GameDifficulty::HARD) hp = 1000;
    
    state = BossState::IDLE;
    stateTimer = 0;
    bullets.clear();
}

void Boss::Update(VECTOR playerPos) {
    stateTimer++;

    switch (state) {
        case BossState::IDLE:
            if (stateTimer > 60) {
                int attackProb = (g_Difficulty == GameDifficulty::HARD) ? 30 : (g_Difficulty == GameDifficulty::EASY) ? 70 : 50;
                if (GetRand(100) < attackProb) {
                    state = BossState::ATTACKING;
                } else {
                    state = BossState::TELEPORTING;
                    PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
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
                
                if (g_Difficulty == GameDifficulty::EASY) b.speed = 5.0f;
                else if (g_Difficulty == GameDifficulty::NORMAL) b.speed = 8.0f;
                else if (g_Difficulty == GameDifficulty::HARD) b.speed = 12.0f;
                
                b.radius = 10.0f;
                b.active = true;
                b.isReflected = false;
                bullets.push_back(b);
            }
            if (stateTimer > ((g_Difficulty == GameDifficulty::HARD) ? 15 : 30)) {
                state = BossState::IDLE;
                stateTimer = 0;
            }
            break;
            
        case BossState::TELEPORTING:
            if (stateTimer == 30) {
                pos.x = (float)(GetRand(800) - 400);
                pos.z = (float)(GetRand(300) + 100);
            }
            if (stateTimer > 60) {
                state = BossState::IDLE;
                stateTimer = 0;
            }
            break;
    }

    for (auto& b : bullets) {
        if (b.active) {
            b.pos = VAdd(b.pos, VScale(b.dir, b.speed));
            if (b.pos.z < -600.0f || b.pos.z > 600.0f || b.pos.x < -600.0f || b.pos.x > 600.0f) {
                b.active = false;
            }
        }
    }
}

void Boss::Draw() {
    if (state == BossState::TELEPORTING && stateTimer < 30) {
        if (stateTimer % 4 < 2) {
            VECTOR glitchPos = VAdd(pos, VGet((float)(GetRand(10)-5), (float)(GetRand(10)-5), (float)(GetRand(10)-5)));
            DrawSphere3D(glitchPos, radius, 32, GetColor(0, 255, 255), GetColor(255,255,255), TRUE);
        }
    } else {
        // --- Procedural 3D Model: Mechanical Fortress ---
        
        // Central Hull
        VECTOR top = VAdd(pos, VGet(0, radius * 0.8f, 0));
        VECTOR bottom = VAdd(pos, VGet(0, -radius * 0.5f, 0));
        DrawCapsule3D(top, bottom, radius, 32, GetColor(180, 180, 200), GetColor(255, 255, 255), TRUE);
        
        // Armor plates (rotating ring)
        float armorAngle = stateTimer * 0.05f;
        for (int i = 0; i < 4; i++) {
            float a = armorAngle + (3.14159f / 2.0f) * i;
            VECTOR platePos = VAdd(pos, VGet(cos(a) * radius * 1.4f, 0, sin(a) * radius * 1.4f));
            DrawSphere3D(platePos, radius * 0.4f, 16, GetColor(200, 200, 220), GetColor(255,255,255), TRUE);
            
            VECTOR spikeEnd = VAdd(platePos, VGet(cos(a) * radius * 2.0f, 0, sin(a) * radius * 2.0f));
            DrawCone3D(platePos, spikeEnd, radius * 0.2f, 8, GetColor(150, 0, 0), GetColor(255, 255, 255), TRUE);
        }
        
        // Main eye (glowing red)
        VECTOR eyePos = VAdd(pos, VGet(0, 10.0f, -radius * 0.95f));
        SetDrawBlendMode(DX_BLENDMODE_ADD, 200);
        DrawSphere3D(eyePos, radius * 0.4f, 16, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        DrawSphere3D(eyePos, radius * 0.2f, 16, GetColor(255, 200, 200), GetColor(255, 255, 255), TRUE);
        
        // Floating energy parts
        float energyAngle = -stateTimer * 0.1f;
        VECTOR part1 = VAdd(pos, VGet(cos(energyAngle) * radius * 1.5f, -radius * 0.8f, sin(energyAngle) * radius * 1.5f));
        VECTOR part2 = VAdd(pos, VGet(cos(energyAngle + 3.14f) * radius * 1.5f, -radius * 0.8f, sin(energyAngle + 3.14f) * radius * 1.5f));
        SetDrawBlendMode(DX_BLENDMODE_ADD, 150);
        DrawSphere3D(part1, radius * 0.2f, 8, GetColor(0, 255, 255), GetColor(255,255,255), TRUE);
        DrawSphere3D(part2, radius * 0.2f, 8, GetColor(0, 255, 255), GetColor(255,255,255), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    for (const auto& b : bullets) {
        if (b.active) {
            int color = b.isReflected ? GetColor(0, 255, 255) : GetColor(255, 50, 0);
            VECTOR endPos = VAdd(b.pos, VScale(b.dir, -20.0f));
            DrawCapsule3D(b.pos, endPos, b.radius, 8, color, GetColor(255,255,255), TRUE);
        }
    }
}
