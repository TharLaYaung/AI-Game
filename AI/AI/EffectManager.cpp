#include "EffectManager.h"
#include <math.h>

EffectManager::EffectManager() {}

void EffectManager::Initialize() {
    particles.clear();
}

void EffectManager::Update() {
    for (auto& p : particles) {
        if (p.active) {
            p.pos = VAdd(p.pos, p.vel);
            p.life--;
            if (p.life <= 0) p.active = false;
        }
    }
}

void EffectManager::Draw() {
    for (const auto& p : particles) {
        if (p.active) {
            int alpha = (int)(255.0f * ((float)p.life / p.maxLife));
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
            DrawSphere3D(p.pos, p.size, 8, p.color, GetColor(255,255,255), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }
}

void EffectManager::AddExplosion(VECTOR pos, int color, int count) {
    for (int i = 0; i < count; i++) {
        Particle p;
        p.pos = pos;
        
        float angleY = (float)GetRand(314) / 50.0f;
        float angleP = (float)(GetRand(314) - 157) / 50.0f;
        float speed = (float)(GetRand(10) + 5);
        
        p.vel = VGet(cos(angleP) * sin(angleY) * speed,
                     sin(angleP) * speed,
                     cos(angleP) * cos(angleY) * speed);
                     
        p.life = GetRand(20) + 10;
        p.maxLife = p.life;
        p.color = color;
        p.size = (float)(GetRand(5) + 3);
        p.active = true;
        
        particles.push_back(p);
    }
}

void EffectManager::AddTeleport(VECTOR pos) {
    for (int i = 0; i < 40; i++) {
        Particle p;
        p.pos = VAdd(pos, VGet((float)(GetRand(40)-20), (float)(GetRand(40)-20), (float)(GetRand(40)-20)));
        p.vel = VGet(0.0f, (float)(GetRand(5)), 0.0f);
        p.life = GetRand(20) + 10;
        p.maxLife = p.life;
        p.color = GetRand(100) > 50 ? GetColor(0, 255, 255) : GetColor(255, 0, 255);
        p.size = (float)(GetRand(4) + 1);
        p.active = true;
        particles.push_back(p);
    }
}
