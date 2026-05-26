#include "Player.h"
#include "GameTypes.h"
#include "DxLib.h"

Player::Player() {}

void Player::Initialize() {
    pos = VGet(0.0f, 0.0f, 0.0f);
    speed = 5.0f;
    radius = 20.0f;
    hp = 100;
    maxBarrierGauge = 100.0f;
    barrierGauge = 100.0f;
    currentForm = PlayerForm::ATTACK;
    muzzleFlashTimer = 0;
    shieldBuffTimer = 0;
    laserWeaponTimer = 0;
    laserHeat = 0.0f;
    overheatTimer = 0;
    isFiringLaser = false;
    bullets.clear();
}

void Player::Update() {
    if (shieldBuffTimer > 0) shieldBuffTimer--;

    float moveSpeed = 5.0f;
    if (g_Buff == BuffType::SPEED) moveSpeed = (g_BuffLevel == 2) ? 12.0f : 8.0f;

    if (CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_UP))    pos.z += moveSpeed;
    if (CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_DOWN))  pos.z -= moveSpeed;
    if (CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_LEFT))  pos.x -= moveSpeed;
    if (CheckHitKey(KEY_INPUT_D) || CheckHitKey(KEY_INPUT_RIGHT)) pos.x += moveSpeed;
    if (CheckHitKey(KEY_INPUT_Q)) pos.y += moveSpeed;
    if (CheckHitKey(KEY_INPUT_E)) pos.y -= moveSpeed;

    if (pos.x < -600.0f) pos.x = -600.0f;
    if (pos.x > 600.0f)  pos.x = 600.0f;
    if (pos.y < -200.0f) pos.y = -200.0f;
    if (pos.y > 600.0f)  pos.y = 600.0f;
    if (pos.z < -400.0f) pos.z = -400.0f;
    if (pos.z > 400.0f)  pos.z = 400.0f;

    static bool prevShift = false;
    bool currShift = CheckHitKey(KEY_INPUT_LSHIFT) || CheckHitKey(KEY_INPUT_RSHIFT) || (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0;
    if (currShift && !prevShift) {
        if (currentForm == PlayerForm::ATTACK) {
            if (barrierGauge > 20.0f) {
                currentForm = PlayerForm::BARRIER;
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Insert.wav", DX_PLAYTYPE_BACK);
            }
        } else {
            currentForm = PlayerForm::ATTACK;
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
        }
    }
    prevShift = currShift;

    
    if (currentForm == PlayerForm::BARRIER) {
        barrierGauge -= 0.15f;
        if (barrierGauge <= 0.0f) {
            barrierGauge = 0.0f;
            currentForm = PlayerForm::ATTACK;
            PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Remove.wav", DX_PLAYTYPE_BACK);
        }
    } else {
        barrierGauge += 0.2f;
        if (barrierGauge > maxBarrierGauge) barrierGauge = maxBarrierGauge;
    }

    static bool prevSpace = false;
    bool currSpace = CheckHitKey(KEY_INPUT_SPACE) || (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
    
    if (laserWeaponTimer > 0) laserWeaponTimer--;
    
    isFiringLaser = false;
    if (overheatTimer > 0) {
        overheatTimer--;
        laserHeat -= 100.0f / 180.0f; // Cool down over 3 seconds
        if (laserHeat < 0.0f) laserHeat = 0.0f;
    } else {
        if (laserWeaponTimer > 0 && currSpace && currentForm == PlayerForm::ATTACK) {
            isFiringLaser = true;
            laserHeat += 100.0f / 120.0f; // Overheats in 2 seconds
            if (laserHeat >= 100.0f) {
                laserHeat = 100.0f;
                overheatTimer = 180; // 3 seconds penalty
                isFiringLaser = false;
                PlaySoundFile(L"C:\\Windows\\Media\\Windows Hardware Fail.wav", DX_PLAYTYPE_BACK);
            }
        } else {
            laserHeat -= 100.0f / 60.0f; // Fast cool down (1 sec)
            if (laserHeat < 0.0f) laserHeat = 0.0f;
        }
    }
    
    static int attackCooldown = 0;
    int cooldownLimit = 10;
    if (g_Buff == BuffType::SPEED) cooldownLimit = (g_BuffLevel == 2) ? 3 : 5;
    attackCooldown++;
    
    if (muzzleFlashTimer > 0) muzzleFlashTimer--;
    
    if (currSpace && currentForm == PlayerForm::ATTACK && attackCooldown > cooldownLimit && laserWeaponTimer <= 0 && overheatTimer <= 0) {
        attackCooldown = 0;
        muzzleFlashTimer = 5;
        Bullet b;
        b.pos = pos;
        b.dir = VGet(0, 0, 1.0f);
        b.speed = (g_Buff == BuffType::SPEED) ? 25.0f : 15.0f;
        b.radius = (g_Buff == BuffType::BOMB) ? 15.0f : ((g_Buff == BuffType::LASER && g_BuffLevel == 2) ? 10.0f : 5.0f);
        b.active = true;
        b.isReflected = false;
        bullets.push_back(b);
        PlaySoundFile(L"C:\\Windows\\Media\\Windows Ding.wav", DX_PLAYTYPE_BACK);
    }
    prevSpace = currSpace;

    for (auto& b : bullets) {
        if (b.active) {
            b.pos = VAdd(b.pos, VScale(b.dir, b.speed));
            if (b.pos.z > 1000.0f) b.active = false;
        }
    }
}

void Player::Draw() {
    int coreColor = (currentForm == PlayerForm::ATTACK) ? GetColor(255, 50, 50) : GetColor(50, 200, 255);
    
    
    
    VECTOR front = VAdd(pos, VGet(0, 0, radius * 1.5f));
    VECTOR back = VAdd(pos, VGet(0, 0, -radius));
    DrawCapsule3D(front, back, radius * 0.5f, 16, GetColor(200, 200, 220), GetColor(255, 255, 255), TRUE);
    
    
    VECTOR leftWingEnd = VAdd(pos, VGet(-radius * 2.0f, -radius * 0.2f, -radius * 0.5f));
    VECTOR rightWingEnd = VAdd(pos, VGet(radius * 2.0f, -radius * 0.2f, -radius * 0.5f));
    DrawCapsule3D(pos, leftWingEnd, radius * 0.3f, 8, GetColor(150, 150, 170), GetColor(255, 255, 255), TRUE);
    DrawCapsule3D(pos, rightWingEnd, radius * 0.3f, 8, GetColor(150, 150, 170), GetColor(255, 255, 255), TRUE);
    
    
    VECTOR leftThruster = VAdd(leftWingEnd, VGet(0, 0, -radius * 0.8f));
    VECTOR rightThruster = VAdd(rightWingEnd, VGet(0, 0, -radius * 0.8f));
    SetDrawBlendMode(DX_BLENDMODE_ADD, 200);
    DrawCone3D(leftWingEnd, leftThruster, radius * 0.6f, 8, coreColor, GetColor(255, 255, 255), TRUE);
    DrawCone3D(rightWingEnd, rightThruster, radius * 0.6f, 8, coreColor, GetColor(255, 255, 255), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    
    
    VECTOR cockpitPos = VAdd(pos, VGet(0, radius * 0.4f, radius * 0.5f));
    DrawSphere3D(cockpitPos, radius * 0.4f, 16, coreColor, GetColor(255, 255, 255), TRUE);

    if (currentForm == PlayerForm::BARRIER) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
        DrawSphere3D(pos, radius * 2.0f, 16, GetColor(0, 255, 255), GetColor(255,255,255), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    } else {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
        DrawSphere3D(pos, radius * 1.5f, 16, GetColor(255, 0, 0), GetColor(255,255,255), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    if (shieldBuffTimer > 0) {
        
        int glow = 150 + (int)(sin(GetNowCount() * 0.01f) * 100);
        SetDrawBlendMode(DX_BLENDMODE_ADD, glow);
        DrawSphere3D(pos, radius * 3.0f, 16, GetColor(0, 150, 255), GetColor(255, 255, 255), FALSE); 
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    
    if (muzzleFlashTimer > 0) {
        SetDrawBlendMode(DX_BLENDMODE_ADD, muzzleFlashTimer * 50);
        DrawSphere3D(VAdd(pos, VGet(0, 0, radius * 1.5f)), radius * 2.0f, 16, GetColor(255, 200, 50), GetColor(255, 255, 255), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
    
    if (isFiringLaser) {
        VECTOR endPos = VAdd(pos, VGet(0, 0, 1500.0f));
        int rJitter = GetRand(10) - 5;
        SetDrawBlendMode(DX_BLENDMODE_ADD, 200);
        DrawCylinder3D(pos, endPos, 30.0f + rJitter, 16, GetColor(0, 255, 255), GetColor(0, 255, 255), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        DrawCylinder3D(pos, endPos, 15.0f, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    int bulletColor = GetColor(0, 200, 255); 
    if (g_Buff == BuffType::LASER) bulletColor = (g_BuffLevel == 2) ? GetColor(255, 150, 255) : GetColor(255, 50, 255); 
    else if (g_Buff == BuffType::BOMB) bulletColor = (g_BuffLevel == 2) ? GetColor(255, 200, 0) : GetColor(255, 100, 0); 
    else if (g_Buff == BuffType::SPEED) bulletColor = (g_BuffLevel == 2) ? GetColor(100, 255, 150) : GetColor(50, 255, 100); 

    for (const auto& b : bullets) {
        if (b.active) {
            VECTOR endPos = VAdd(b.pos, VScale(b.dir, -30.0f)); 
            
            
            SetDrawBlendMode(DX_BLENDMODE_ADD, 150);
            DrawCapsule3D(b.pos, endPos, b.radius * 2.5f, 8, bulletColor, GetColor(255,255,255), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            
            
            DrawCapsule3D(b.pos, endPos, b.radius, 8, GetColor(255, 255, 255), GetColor(255,255,255), TRUE);
        }
    }
}
