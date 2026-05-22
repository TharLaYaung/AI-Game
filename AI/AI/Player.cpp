#include "Player.h"

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
    if (CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_UP))    pos.z += speed;
    if (CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_DOWN))  pos.z -= speed;
    if (CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_LEFT))  pos.x -= speed;
    if (CheckHitKey(KEY_INPUT_D) || CheckHitKey(KEY_INPUT_RIGHT)) pos.x += speed;

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
    if (currSpace && !prevSpace && currentForm == PlayerForm::ATTACK) {
        Bullet b;
        b.pos = pos;
        b.dir = VGet(0, 0, 1.0f);
        b.speed = 15.0f;
        b.radius = 5.0f;
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
    
    DrawSphere3D(pos, radius, 16, coreColor, GetColor(255,255,255), TRUE);

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
