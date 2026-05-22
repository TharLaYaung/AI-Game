#include "Player.h"
#include "GameTypes.h"
#include "DxLib.h"

Player::Player() {}

void Player::Initialize() {
    pos = VGet(0.0f, 0.0f, 0.0f);
    speed = 5.0f;
    radius = 20.0f;
    hp = 100;
    currentForm = PlayerForm::ATTACK;
    bullets.clear();
}

void Player::Update() {
    float moveSpeed = (g_Buff == BuffType::SPEED) ? 8.0f : 5.0f;

    if (CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_UP))    pos.z += moveSpeed;
    if (CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_DOWN))  pos.z -= moveSpeed;
    if (CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_LEFT))  pos.x -= moveSpeed;
    if (CheckHitKey(KEY_INPUT_D) || CheckHitKey(KEY_INPUT_RIGHT)) pos.x += moveSpeed;

    if (pos.x < -400.0f) pos.x = -400.0f;
    if (pos.x > 400.0f)  pos.x = 400.0f;
    if (pos.z < -400.0f) pos.z = -400.0f;
    if (pos.z > 400.0f)  pos.z = 400.0f;

    static bool prevShift = false;
    bool currShift = CheckHitKey(KEY_INPUT_LSHIFT) || CheckHitKey(KEY_INPUT_RSHIFT);
    if (currShift && !prevShift) {
        if (currentForm == PlayerForm::ATTACK) currentForm = PlayerForm::BARRIER;
        else currentForm = PlayerForm::ATTACK;
    }
    prevShift = currShift;

    static bool prevSpace = false;
    bool currSpace = CheckHitKey(KEY_INPUT_SPACE);
    
    // Attack rate based on Speed buff
    static int attackCooldown = 0;
    int cooldownLimit = (g_Buff == BuffType::SPEED) ? 5 : 10;
    attackCooldown++;
    
    if (currSpace && currentForm == PlayerForm::ATTACK && attackCooldown > cooldownLimit) {
        attackCooldown = 0;
        Bullet b;
        b.pos = pos;
        b.dir = VGet(0, 0, 1.0f);
        b.speed = (g_Buff == BuffType::SPEED) ? 25.0f : 15.0f;
        b.radius = (g_Buff == BuffType::BOMB) ? 15.0f : 5.0f; // BOMB has bigger bullets
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
    
    // --- Procedural 3D Model: Sci-Fi Fighter ---
    // Main Body
    VECTOR front = VAdd(pos, VGet(0, 0, radius * 1.5f));
    VECTOR back = VAdd(pos, VGet(0, 0, -radius));
    DrawCapsule3D(front, back, radius * 0.5f, 16, GetColor(200, 200, 220), GetColor(255, 255, 255), TRUE);
    
    // Wings
    VECTOR leftWingEnd = VAdd(pos, VGet(-radius * 2.0f, -radius * 0.2f, -radius * 0.5f));
    VECTOR rightWingEnd = VAdd(pos, VGet(radius * 2.0f, -radius * 0.2f, -radius * 0.5f));
    DrawCapsule3D(pos, leftWingEnd, radius * 0.3f, 8, GetColor(150, 150, 170), GetColor(255, 255, 255), TRUE);
    DrawCapsule3D(pos, rightWingEnd, radius * 0.3f, 8, GetColor(150, 150, 170), GetColor(255, 255, 255), TRUE);
    
    // Thruster glow
    VECTOR leftThruster = VAdd(leftWingEnd, VGet(0, 0, -radius * 0.8f));
    VECTOR rightThruster = VAdd(rightWingEnd, VGet(0, 0, -radius * 0.8f));
    SetDrawBlendMode(DX_BLENDMODE_ADD, 200);
    DrawCone3D(leftWingEnd, leftThruster, radius * 0.6f, 8, coreColor, GetColor(255, 255, 255), TRUE);
    DrawCone3D(rightWingEnd, rightThruster, radius * 0.6f, 8, coreColor, GetColor(255, 255, 255), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    
    // Cockpit
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

    for (const auto& b : bullets) {
        if (b.active) {
            VECTOR endPos = VAdd(b.pos, VScale(b.dir, -15.0f));
            DrawCapsule3D(b.pos, endPos, b.radius, 8, GetColor(255, 255, 100), GetColor(255,255,255), TRUE);
        }
    }
}
