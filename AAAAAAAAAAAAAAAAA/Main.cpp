#include "DxLib.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

// ============================================================================
// 【Common.h 相当】共通のデータ構造、フォント、定数の定義
// ============================================================================

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// グローバルフォントハンドル
int g_FontTitle = -1;
int g_FontUI = -1;
int g_FontSmall = -1;

enum class SceneType {
    TITLE,
    GAME,
    RESULT,
    GAMEOVER
};

struct GameData {
    int possessedItems = 0;
    int finalScore = 0;
    bool isGameCleared = false;
};

struct RankingEntry {
    std::string name;
    int score;
};

// グローバルなランキングデータ
extern std::vector<RankingEntry> g_Ranking;
std::vector<RankingEntry> g_Ranking = {
    { "X-99", 8000 },
    { "A-01", 5000 },
    { "B-05", 2000 }
};

// ============================================================================
// 【Effects】エフェクト（粒子・星）用データ構造
// ============================================================================

// 粒子（パーティクル）構造体
struct Particle {
    VECTOR position;
    VECTOR velocity;
    unsigned int color;
    float size;
    int life;
    int maxLife;
};

// 背景を流れるスターダスト用構造体
struct Star {
    VECTOR position;
    float speed;
};

// ============================================================================
// 【Input.h 相当】キー入力を1括管理するクラス
// ============================================================================
class Input {
private:
    char currentKeys[256];
    char previousKeys[256];

public:
    Input() {
        for (int i = 0; i < 256; ++i) {
            currentKeys[i] = 0;
            previousKeys[i] = 0;
        }
    }

    void Update() {
        for (int i = 0; i < 256; ++i) {
            previousKeys[i] = currentKeys[i];
        }
        GetHitKeyStateAll(currentKeys);
    }

    bool IsPress(int keyCode) const {
        return currentKeys[keyCode] == 1;
    }

    bool IsTrigger(int keyCode) const {
        return (currentKeys[keyCode] == 1) && (previousKeys[keyCode] == 0);
    }
};

// ============================================================================
// 【Scene.h 相当】すべてのシーンの基盤となる抽象クラス
// ============================================================================
class SceneManager;

class Scene {
protected:
    SceneManager* m_pSceneManager;

public:
    Scene(SceneManager* manager) : m_pSceneManager(manager) {}
    virtual ~Scene() {}

    virtual void Initialize() = 0;
    virtual void Update(const Input& input) = 0;
    virtual void Draw() = 0;
};

// ============================================================================
// 【SceneManager.h 相当】シーンを管理・遷移させるクラス
// ============================================================================
class SceneManager {
private:
    Scene* m_pCurrentScene;
    GameData m_GameData;

public:
    SceneManager() : m_pCurrentScene(nullptr) {}

    ~SceneManager() {
        if (m_pCurrentScene) {
            delete m_pCurrentScene;
        }
    }

    void ChangeScene(SceneType nextScene);

    void Update(const Input& input) {
        if (m_pCurrentScene) {
            m_pCurrentScene->Update(input);
        }
    }

    void Draw() {
        if (m_pCurrentScene) {
            m_pCurrentScene->Draw();
        }
    }

    GameData& GetGameData() { return m_GameData; }
};

// ============================================================================
// 【GameObject.h 相当】プレイヤーとボスの3D定義
// ============================================================================

enum class PlayerMode {
    ATTACK, // 攻撃モード
    BARRIER // バリアモード
};

struct Player {
    VECTOR position;
    float speed;
    int hp;
    int maxHp;
    float displayedHp; // スムーズなHPバーアニメーション用
    PlayerMode mode;
    int items;
    float hoverOffset;  // ホバー用(アニメーション)

    void Initialize() {
        position = VGet(0.0f, 0.0f, 0.0f);
        speed = 0.14f;
        hp = 100;
        maxHp = 100;
        displayedHp = 100.0f;
        mode = PlayerMode::ATTACK;
        items = 0;
        hoverOffset = 0.0f;
    }
};

struct Boss {
    VECTOR position;
    int hp;
    int maxHp;
    float displayedHp; // スムーズなHPバーアニメーション用
    int teleportTimer;
    bool isTeleporting;
    float hoverOffset; // ホバー用(アニメーション)

    void Initialize() {
        position = VGet(0.0f, 0.0f, 8.0f);
        hp = 200;
        maxHp = 200;
        displayedHp = 200.0f;
        teleportTimer = 0;
        isTeleporting = false;
        hoverOffset = 0.0f;
    }
};

struct Bullet {
    VECTOR position;
    VECTOR velocity;
    bool isReflected;
    bool isActive;
};

// ============================================================================
// 【TitleScene】タイトル画面
// ============================================================================
class TitleScene : public Scene {
private:
    float m_AnimTimer;
    std::vector<Star> m_Stars;

public:
    TitleScene(SceneManager* manager) : Scene(manager), m_AnimTimer(0.0f) {}

    void Initialize() override {
        m_pSceneManager->GetGameData().possessedItems = 0;
        m_pSceneManager->GetGameData().finalScore = 0;
        m_pSceneManager->GetGameData().isGameCleared = false;
        m_AnimTimer = 0.0f;

        // 背景用の簡易的な星
        m_Stars.clear();
        for (int i = 0; i < 40; ++i) {
            Star s;
            s.position = VGet((float)(GetRand(SCREEN_WIDTH)), (float)(GetRand(SCREEN_HEIGHT)), 0);
            s.speed = 0.5f + (float)GetRand(15) / 10.0f;
            m_Stars.push_back(s);
        }
    }

    void Update(const Input& input) override {
        m_AnimTimer += 0.05f;

        // 星を流す
        for (auto& s : m_Stars) {
            s.position.x -= s.speed;
            if (s.position.x < -10) {
                s.position.x = SCREEN_WIDTH + 10;
                s.position.y = (float)(GetRand(SCREEN_HEIGHT));
            }
        }

        if (input.IsTrigger(KEY_INPUT_RETURN)) {
            m_pSceneManager->ChangeScene(SceneType::GAME);
        }
    }

    void Draw() override {
        // 電脳調のダークブルー背景
        DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(5, 5, 12), TRUE);

        // スターダストの描画
        for (const auto& s : m_Stars) {
            unsigned int starColor = GetColor(100, 150, 255);
            if (s.speed > 1.5f) starColor = GetColor(255, 255, 255);
            DrawCircle((int)s.position.x, (int)s.position.y, (int)(s.speed * 0.8f), starColor, TRUE);
        }

        // タイトルロゴ (文字の周りにネオン風の二重線フレームを描画)
        DrawBox(100, 100, SCREEN_WIDTH - 100, 250, GetColor(16, 185, 129), FALSE);
        DrawBox(103, 103, SCREEN_WIDTH - 103, 247, GetColor(59, 130, 246), FALSE);

        // テキスト描画 (作成したアンチエイリアスフォントを利用)
        DrawFormatStringToHandle(180, 135, GetColor(255, 255, 255), g_FontTitle, "CYBER DEFENDER 3D");
        DrawFormatStringToHandle(220, 195, GetColor(56, 189, 248), g_FontUI, "～ SHIELD SWITCHING ROBOT ACTION ～");

        // スタートガイド (点滅アニメーション)
        unsigned int alpha = (unsigned int)(127 + 128 * std::sin(m_AnimTimer * 2.0f));
        unsigned int startColor = GetColor(251, 146, 60);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawFormatStringToHandle(250, 330, startColor, g_FontTitle, "[ PRESS ENTER TO START ]");
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // 操作説明枠
        DrawBox(150, 400, SCREEN_WIDTH - 150, 550, GetColor(15, 23, 42), TRUE);
        DrawBox(150, 400, SCREEN_WIDTH - 150, 550, GetColor(30, 41, 59), FALSE);

        DrawFormatStringToHandle(170, 415, GetColor(16, 185, 129), g_FontUI, "【OPERATIONS】");
        DrawFormatStringToHandle(170, 445, GetColor(226, 232, 240), g_FontSmall, "・MOVE        : [W][A][S][D] / [ARROW KEYS]");
        DrawFormatStringToHandle(170, 470, GetColor(226, 232, 240), g_FontSmall, "・MODE SWITCH : [SPACE KEY] (ATTACK / BARRIER)");
        DrawFormatStringToHandle(170, 495, GetColor(226, 232, 240), g_FontSmall, "・BARRIER     : REFLECT enemy's bullets to deal double damage!");
        DrawFormatStringToHandle(170, 520, GetColor(239, 68, 68), g_FontSmall, "・QUIT GAME   : [ESC KEY]");
    }
};

// ============================================================================
// 【GameScene】メインゲーム画面
// ============================================================================
class GameScene : public Scene {
private:
    Player m_Player;
    Boss m_Boss;
    std::vector<Bullet> m_Bullets;
    std::vector<Particle> m_Particles;
    std::vector<Star> m_Stars3D;

    int m_FireTimer;
    int m_BossFireTimer;
    int m_Score;
    float m_AnimTimer;

    // 画面揺れ効果
    int m_ShakeTimer;

    const float LIMIT_X = 5.0f;
    const float LIMIT_Y = 3.5f;

public:
    GameScene(SceneManager* manager)
        : Scene(manager), m_FireTimer(0), m_BossFireTimer(0), m_Score(0), m_AnimTimer(0.0f), m_ShakeTimer(0) {
    }

    void Initialize() override {
        m_Player.Initialize();
        m_Boss.Initialize();
        m_Bullets.clear();
        m_Particles.clear();
        m_Score = 0;
        m_AnimTimer = 0.0f;
        m_ShakeTimer = 0;

        // 3D用の星屑背景を生成 (奥行き感の表現)
        m_Stars3D.clear();
        for (int i = 0; i < 150; ++i) {
            Star s;
            s.position = VGet(
                (float)(GetRand(200) - 100) / 10.0f,
                (float)(GetRand(100) - 50) / 10.0f,
                (float)(GetRand(250)) / 10.0f
            );
            s.speed = 0.05f + (float)GetRand(5) / 100.0f;
            m_Stars3D.push_back(s);
        }

        // カメラ初期位置 (ロボットを斜め上から俯瞰)
        SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 4.0f, -8.0f), VGet(0.0f, 0.0f, 3.0f));
        SetCameraNearFar(0.1f, 100.0f);

        // --------------------------------------------------------------------
        // ライティングの劇的強化 (画面を明るく、立体的に見せる)
        // --------------------------------------------------------------------
        SetUseLighting(TRUE);
        // 環境光（影の部分）を明るく調整
        SetLightAmbColor(GetColorF(0.45f, 0.45f, 0.55f, 1.0f));
        // ディレクショナルライト（太陽光）を白く強く当てる
        SetLightDifColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));
        // 右上後方からのライティングでエッジを立たせる
        SetLightDirection(VGet(-1.0f, -1.5f, 1.5f));
    }

    // パーティクルの追加
    void AddExplosion(VECTOR pos, int count, unsigned int color, float force = 0.25f) {
        for (int i = 0; i < count; ++i) {
            Particle p;
            p.position = pos;
            p.velocity = VGet(
                ((float)GetRand(200) - 100.0f) / 100.0f * force,
                ((float)GetRand(200) - 100.0f) / 100.0f * force,
                ((float)GetRand(200) - 100.0f) / 100.0f * force
            );
            p.color = color;
            p.size = 0.04f + (float)GetRand(10) / 100.0f;
            p.life = 0;
            p.maxLife = 15 + GetRand(20);
            m_Particles.push_back(p);
        }
    }

    void Update(const Input& input) override {
        m_AnimTimer += 0.1f;

        // --------------------------------------------------------------------
        // 1. 移動処理 (ホバーの揺らぎ計算を含む)
        // --------------------------------------------------------------------
        m_Player.hoverOffset = std::sin(m_AnimTimer * 0.3f) * 0.1f;
        m_Boss.hoverOffset = std::sin(m_AnimTimer * 0.2f) * 0.15f;

        float dx = 0.0f;
        float dy = 0.0f;

        float currentSpeed = m_Player.speed;
        if (m_Player.mode == PlayerMode::BARRIER) {
            currentSpeed *= 0.55f; // バリア展開中はスピード低下
        }

        if (input.IsPress(KEY_INPUT_A) || input.IsPress(KEY_INPUT_LEFT))  dx -= 1.0f;
        if (input.IsPress(KEY_INPUT_D) || input.IsPress(KEY_INPUT_RIGHT)) dx += 1.0f;
        if (input.IsPress(KEY_INPUT_S) || input.IsPress(KEY_INPUT_DOWN))  dy -= 1.0f;
        if (input.IsPress(KEY_INPUT_W) || input.IsPress(KEY_INPUT_UP))    dy += 1.0f;

        // 斜め移動時の等速化
        if (dx != 0.0f && dy != 0.0f) {
            float length = std::sqrt(dx * dx + dy * dy);
            dx /= length;
            dy /= length;
        }

        m_Player.position.x += dx * currentSpeed;
        m_Player.position.y += dy * currentSpeed;

        // 画面外への進行ガード
        if (m_Player.position.x < -LIMIT_X) m_Player.position.x = -LIMIT_X;
        if (m_Player.position.x > LIMIT_X) m_Player.position.x = LIMIT_X;
        if (m_Player.position.y < -LIMIT_Y) m_Player.position.y = -LIMIT_Y;
        if (m_Player.position.y > LIMIT_Y) m_Player.position.y = LIMIT_Y;

        // HPバーの減衰表示アニメーション (ヌルッと動かす)
        m_Player.displayedHp += ((float)m_Player.hp - m_Player.displayedHp) * 0.1f;
        m_Boss.displayedHp += ((float)m_Boss.hp - m_Boss.displayedHp) * 0.1f;

        // --------------------------------------------------------------------
        // 2. モード切り替え (バリア ⇔ 攻撃)
        // --------------------------------------------------------------------
        if (input.IsTrigger(KEY_INPUT_SPACE)) {
            if (m_Player.mode == PlayerMode::ATTACK) {
                m_Player.mode = PlayerMode::BARRIER;
                // バリア切り替え時にフラッシュ効果
                AddExplosion(m_Player.position, 15, GetColor(52, 211, 153), 0.12f);
            }
            else {
                m_Player.mode = PlayerMode::ATTACK;
            }
        }

        // --------------------------------------------------------------------
        // 3. ボスの瞬間移動
        // --------------------------------------------------------------------
        m_Boss.teleportTimer++;
        if (m_Boss.teleportTimer > 150) {
            m_Boss.teleportTimer = 0;

            // 古い位置でエフェクト
            AddExplosion(m_Boss.position, 20, GetColor(168, 85, 247), 0.2f);

            // ランダムにワープ
            m_Boss.position.x = (float)(GetRand(80) - 40) / 10.0f;
            m_Boss.position.y = (float)(GetRand(50) - 25) / 10.0f;

            // 新しい位置で出現エフェクト
            AddExplosion(m_Boss.position, 20, GetColor(192, 132, 252), 0.15f);

            // 稀にアイテムドロップ
            if (GetRand(10) > 6) {
                m_Player.items++;
                m_Score += 300;
            }
        }

        // --------------------------------------------------------------------
        // 4. 背景(星屑)の進行アニメーション
        // --------------------------------------------------------------------
        for (auto& s : m_Stars3D) {
            s.position.z -= s.speed;
            if (s.position.z < -2.0f) {
                s.position.z = 25.0f;
                s.position.x = (float)(GetRand(200) - 100) / 10.0f;
                s.position.y = (float)(GetRand(100) - 50) / 10.0f;
            }
        }

        // --------------------------------------------------------------------
        // 5. 射撃・弾生成
        // --------------------------------------------------------------------
        // プレイヤーのショット
        if (m_Player.mode == PlayerMode::ATTACK) {
            m_FireTimer++;
            if (m_FireTimer > 8) {
                m_FireTimer = 0;
                Bullet b;
                b.position = VAdd(m_Player.position, VGet(0.0f, m_Player.hoverOffset, 0.2f));
                b.velocity = VGet(0.0f, 0.0f, 0.35f);
                b.isReflected = true;
                b.isActive = true;
                m_Bullets.push_back(b);
            }
        }

        // ボスの弾幕ショット
        m_BossFireTimer++;
        if (m_BossFireTimer > 35) {
            m_BossFireTimer = 0;

            // 自機に向かって射撃
            VECTOR dir = VSub(VAdd(m_Player.position, VGet(0.0f, m_Player.hoverOffset, 0.0f)), m_Boss.position);
            float len = VSize(dir);
            if (len > 0.0f) {
                dir.x /= len; dir.y /= len; dir.z /= len;
            }

            Bullet b;
            b.position = VAdd(m_Boss.position, VGet(0.0f, m_Boss.hoverOffset, -0.4f));
            b.velocity = VScale(dir, 0.14f);
            b.isReflected = false;
            b.isActive = true;
            m_Bullets.push_back(b);
        }

        // --------------------------------------------------------------------
        // 6. 弾の更新と衝突判定
        // --------------------------------------------------------------------
        for (auto& b : m_Bullets) {
            if (!b.isActive) continue;

            b.position = VAdd(b.position, b.velocity);

            // 射程限界外
            if (b.position.z > 25.0f || b.position.z < -5.0f) {
                b.isActive = false;
                continue;
            }

            if (!b.isReflected) {
                // ボスの弾 vs プレイヤー
                float dist = VSize(VSub(b.position, VAdd(m_Player.position, VGet(0.0f, m_Player.hoverOffset, 0.0f))));
                if (dist < 0.65f) {
                    b.isActive = false;

                    if (m_Player.mode == PlayerMode::BARRIER) {
                        // 【バリア反射】弾道反転
                        b.isReflected = true;
                        VECTOR toBoss = VSub(VAdd(m_Boss.position, VGet(0.0f, m_Boss.hoverOffset, 0.0f)), b.position);
                        float l = VSize(toBoss);
                        if (l > 0.0f) {
                            float speedMultiplier = 0.42f / l;
                            b.velocity.x = toBoss.x * speedMultiplier;
                            b.velocity.y = toBoss.y * speedMultiplier;
                            b.velocity.z = toBoss.z * speedMultiplier;
                        }
                        b.isActive = true;
                        m_Score += 150;
                        AddExplosion(b.position, 12, GetColor(52, 211, 153), 0.1f); // バリア火花
                    }
                    else {
                        // ダメージ被弾
                        m_Player.hp -= 20;
                        m_ShakeTimer = 15; // 画面を激しく揺らす
                        AddExplosion(b.position, 15, GetColor(239, 68, 68), 0.28f); // 爆発火花

                        if (m_Player.hp <= 0) {
                            m_Player.hp = 0;
                            m_pSceneManager->GetGameData().finalScore = m_Score;
                            m_pSceneManager->GetGameData().isGameCleared = false;
                            m_pSceneManager->ChangeScene(SceneType::GAMEOVER);
                            return;
                        }
                    }
                }
            }
            else {
                // プレイヤー(または反射弾) vs ボス
                float dist = VSize(VSub(b.position, VAdd(m_Boss.position, VGet(0.0f, m_Boss.hoverOffset, 0.0f))));
                if (dist < 1.1f) {
                    b.isActive = false;
                    int damage = b.isReflected ? 25 : 8; // 反射弾の方が威力が高い
                    m_Boss.hp -= damage;
                    m_Score += b.isReflected ? 200 : 50;

                    AddExplosion(b.position, 10, GetColor(254, 240, 138), 0.15f);

                    if (m_Boss.hp <= 0) {
                        m_Boss.hp = 0;
                        m_pSceneManager->GetGameData().possessedItems = m_Player.items;
                        m_pSceneManager->GetGameData().finalScore = m_Score + (m_Player.hp * 20) + (m_Player.items * 1000);
                        m_pSceneManager->GetGameData().isGameCleared = true;
                        m_pSceneManager->ChangeScene(SceneType::RESULT);
                        return;
                    }
                }
            }
        }

        // --------------------------------------------------------------------
        // 7. パーティクルの更新
        // --------------------------------------------------------------------
        for (auto& p : m_Particles) {
            p.position = VAdd(p.position, p.velocity);
            p.life++;
        }
        m_Particles.erase(std::remove_if(m_Particles.begin(), m_Particles.end(),
            [](const Particle& p) { return p.life >= p.maxLife; }), m_Particles.end());

        // 使用済みの弾をクリーンアップ
        m_Bullets.erase(std::remove_if(m_Bullets.begin(), m_Bullets.end(),
            [](const Bullet& b) { return !b.isActive; }), m_Bullets.end());
    }

    void Draw() override {
        // 深淵なエレクトロニック宇宙背景
        DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(4, 4, 10), TRUE);

        // --------------------------------------------------------------------
        // 画面揺れを含むカメラの設定
        // --------------------------------------------------------------------
        VECTOR camPos = VGet(0.0f, 4.0f, -8.0f);
        VECTOR camTarget = VGet(0.0f, 0.0f, 3.0f);

        if (m_ShakeTimer > 0) {
            float intensity = (float)m_ShakeTimer * 0.06f;
            camPos.x += ((float)GetRand(100) - 50.0f) / 50.0f * intensity;
            camPos.y += ((float)GetRand(100) - 50.0f) / 50.0f * intensity;
            camTarget.x += ((float)GetRand(100) - 50.0f) / 50.0f * intensity;
            m_ShakeTimer--;
        }
        SetCameraPositionAndTarget_UpVecY(camPos, camTarget);

        // --------------------------------------------------------------------
        // 3D背景グリッド
        // --------------------------------------------------------------------
        for (float z = -2.0f; z <= 25.0f; z += 1.5f) {
            DrawLine3D(VGet(-LIMIT_X, -LIMIT_Y - 0.2f, z), VGet(LIMIT_X, -LIMIT_Y - 0.2f, z), GetColor(30, 41, 100));
        }
        for (float x = -LIMIT_X; x <= LIMIT_X; x += 1.5f) {
            DrawLine3D(VGet(x, -LIMIT_Y - 0.2f, -2.0f), VGet(x, -LIMIT_Y - 0.2f, 25.0f), GetColor(30, 41, 100));
        }

        // 外周グリッド枠
        DrawLine3D(VGet(-LIMIT_X, -LIMIT_Y - 0.2f, 0.0f), VGet(LIMIT_X, -LIMIT_Y - 0.2f, 0.0f), GetColor(59, 130, 246));
        DrawLine3D(VGet(-LIMIT_X, LIMIT_Y, 0.0f), VGet(LIMIT_X, LIMIT_Y, 0.0f), GetColor(59, 130, 246));
        DrawLine3D(VGet(-LIMIT_X, -LIMIT_Y - 0.2f, 0.0f), VGet(-LIMIT_X, LIMIT_Y, 0.0f), GetColor(59, 130, 246));
        DrawLine3D(VGet(LIMIT_X, -LIMIT_Y - 0.2f, 0.0f), VGet(LIMIT_X, LIMIT_Y, 0.0f), GetColor(59, 130, 246));

        // --------------------------------------------------------------------
        // 3Dオブジェクトのライティング描画
        // --------------------------------------------------------------------
        SetUseLighting(TRUE);

        // ---------------------------------------------
        // ロボットボス (モノアイを搭載した巨大球型ビット機体)
        // ---------------------------------------------
        VECTOR renderBossPos = VAdd(m_Boss.position, VGet(0.0f, m_Boss.hoverOffset, 0.0f));
        // コアボディ
        DrawSphere3D(renderBossPos, 0.8f, 16, GetColor(220, 38, 38), GetColor(255, 255, 255), TRUE);
        // アーマーフレーム(ワイヤー風の殻)
        DrawSphere3D(renderBossPos, 1.0f, 8, GetColor(71, 85, 105), GetColor(255, 255, 255), FALSE);

        // 周回する軌道追従型ビット(2機)の描画
        float angle1 = m_AnimTimer * 0.1f;
        VECTOR bit1Pos = VAdd(renderBossPos, VGet(std::cos(angle1) * 1.5f, std::sin(angle1) * 0.4f, std::sin(angle1) * 0.6f));
        DrawSphere3D(bit1Pos, 0.22f, 8, GetColor(168, 85, 247), GetColor(255, 255, 255), TRUE);

        float angle2 = angle1 + 3.14159f;
        VECTOR bit2Pos = VAdd(renderBossPos, VGet(std::cos(angle2) * 1.5f, std::sin(angle2) * 0.4f, std::sin(angle2) * 0.6f));
        DrawSphere3D(bit2Pos, 0.22f, 8, GetColor(168, 85, 247), GetColor(255, 255, 255), TRUE);

        // ---------------------------------------------
        // プレイヤーロボット (幾何学パーツを組み合わせたスマートな外形)
        // ---------------------------------------------
        VECTOR renderPlayerPos = VAdd(m_Player.position, VGet(0.0f, m_Player.hoverOffset, 0.0f));
        unsigned int playerColor = (m_Player.mode == PlayerMode::ATTACK) ? GetColor(59, 130, 246) : GetColor(16, 185, 129);

        // 胴体コア
        DrawSphere3D(renderPlayerPos, 0.35f, 16, playerColor, GetColor(255, 255, 255), TRUE);
        // ショルダーアーマー（左・右）
        DrawCapsule3D(VAdd(renderPlayerPos, VGet(-0.48f, 0.02f, 0.0f)), VAdd(renderPlayerPos, VGet(-0.35f, -0.05f, 0.0f)), 0.15f, 8, playerColor, GetColor(255, 255, 255), TRUE);
        DrawCapsule3D(VAdd(renderPlayerPos, VGet(0.35f, -0.05f, 0.0f)), VAdd(renderPlayerPos, VGet(0.48f, 0.02f, 0.0f)), 0.15f, 8, playerColor, GetColor(255, 255, 255), TRUE);

        // --------------------------------------------------------------------
        // 光源を必要としない「自己発光オブジェクト」の描画 (ライティング一時OFF)
        // --------------------------------------------------------------------
        SetUseLighting(FALSE);

        // ボスのモノアイ（常に眩しく赤に発光）
        DrawSphere3D(VAdd(renderBossPos, VGet(0.0, 0.0f, -0.76f)), 0.14f, 8, GetColor(244, 63, 94), GetColor(255, 255, 255), TRUE);

        // プレイヤーのスラスター炎 (ブースター部分が明滅しながら伸縮)
        float thrustLength = 0.25f + 0.18f * std::abs(std::sin(m_AnimTimer * 0.6f));
        DrawCone3D(
            VAdd(renderPlayerPos, VGet(0.0f, -0.2f, -0.25f)),
            VAdd(renderPlayerPos, VGet(0.0f, -0.2f, -0.25f - thrustLength)),
            0.15f, 10, GetColor(251, 146, 60), GetColor(239, 68, 68), TRUE
        );

        // 星屑(3D)の描画
        for (const auto& s : m_Stars3D) {
            DrawSphere3D(s.position, 0.025f, 4, GetColor(148, 163, 184), GetColor(255, 255, 255), TRUE);
        }

        // バリアシールドの描画 (脈動脈動アニメーション＋半透明加算ブレンド)
        if (m_Player.mode == PlayerMode::BARRIER) {
            float pulseScale = 0.68f + 0.06f * std::sin(m_AnimTimer * 0.8f);
            SetDrawBlendMode(DX_BLENDMODE_ADD, 120);
            DrawSphere3D(renderPlayerPos, pulseScale, 16, GetColor(52, 211, 153), GetColor(255, 255, 255), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        // 弾の描画
        for (const auto& b : m_Bullets) {
            unsigned int bulletColor = b.isReflected ? GetColor(56, 189, 248) : GetColor(245, 158, 11);
            float bulletSize = b.isReflected ? 0.18f : 0.12f;
            DrawSphere3D(b.position, bulletSize, 8, bulletColor, GetColor(255, 255, 255), TRUE);
        }

        // パーティクル(火花)の描画
        SetDrawBlendMode(DX_BLENDMODE_ADD, 180);
        for (const auto& p : m_Particles) {
            DrawSphere3D(p.position, p.size, 6, p.color, GetColor(255, 255, 255), TRUE);
        }
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // --------------------------------------------------------------------
        // 2D UI / HUD（格闘ゲームのような美しく近未来感のあるデザイン）
        // --------------------------------------------------------------------

        // 1. プレイヤーHPバー
        DrawBox(30, 30, 280, 52, GetColor(15, 23, 42), TRUE);
        DrawBox(30, 30, 280, 52, GetColor(59, 130, 246), FALSE);
        // 被弾ダメージ残像(白色)
        float displayedHpRate = m_Player.displayedHp / m_Player.maxHp;
        DrawBox(34, 33, 34 + (int)(242 * displayedHpRate), 49, GetColor(255, 255, 255), TRUE);
        // 現在のHP(緑色〜赤色の変化)
        float actualHpRate = (float)m_Player.hp / m_Player.maxHp;
        unsigned int playerHpColor = (actualHpRate > 0.4f) ? GetColor(16, 185, 129) : GetColor(239, 68, 68);
        DrawBox(34, 33, 34 + (int)(242 * actualHpRate), 49, playerHpColor, TRUE);
        DrawFormatStringToHandle(40, 33, GetColor(255, 255, 255), g_FontSmall, "PLAYER H.P. [ %d%% ]", m_Player.hp);

        // 2. ボスHPバー
        DrawBox(520, 30, 770, 52, GetColor(15, 23, 42), TRUE);
        DrawBox(520, 30, 770, 52, GetColor(239, 68, 68), FALSE);
        // ボスダメージ残像
        float bossDispRate = m_Boss.displayedHp / m_Boss.maxHp;
        DrawBox(524, 33, 524 + (int)(242 * bossDispRate), 49, GetColor(255, 255, 255), TRUE);
        // ボス現在HP
        float bossActualRate = (float)m_Boss.hp / m_Boss.maxHp;
        DrawBox(524, 33, 524 + (int)(242 * bossActualRate), 49, GetColor(220, 38, 38), TRUE);
        DrawFormatStringToHandle(530, 33, GetColor(255, 255, 255), g_FontSmall, "BOSS MATRIX [ %d%% ]", (int)(bossActualRate * 100));

        // 3. モードインジケーター (ネオンサイバーデザイン)
        DrawBox(30, 68, 230, 108, GetColor(15, 23, 42), TRUE);
        if (m_Player.mode == PlayerMode::ATTACK) {
            DrawBox(30, 68, 230, 108, GetColor(59, 130, 246), FALSE);
            DrawFormatStringToHandle(50, 76, GetColor(56, 189, 248), g_FontUI, "MODE: ATTACK");
        }
        else {
            DrawBox(30, 68, 230, 108, GetColor(16, 185, 129), FALSE);
            DrawFormatStringToHandle(45, 76, GetColor(52, 211, 153), g_FontUI, "MODE: BARRIER");
        }

        // 4. スコア、獲得アイテム
        DrawFormatStringToHandle(30, 130, GetColor(253, 224, 71), g_FontUI, "SCORE: %06d PTS", m_Score);
        DrawFormatStringToHandle(30, 155, GetColor(192, 132, 252), g_FontSmall, "NANOMACHINE ITEMS: %d", m_Player.items);
        DrawFormatStringToHandle(30, 178, GetColor(148, 163, 184), g_FontSmall, "※ Defeating sub-cores drop nanomachines.");
    }
};

// ============================================================================
// 【ResultScene】クリアリザルト画面
// ============================================================================
class ResultScene : public Scene {
public:
    ResultScene(SceneManager* manager) : Scene(manager) {}

    void Initialize() override {
        int finalScore = m_pSceneManager->GetGameData().finalScore;

        // ランキングの登録とソート
        RankingEntry newEntry = { "PLAYER", finalScore };
        g_Ranking.push_back(newEntry);
        std::sort(g_Ranking.begin(), g_Ranking.end(), [](const RankingEntry& a, const RankingEntry& b) {
            return a.score > b.score;
            });
        if (g_Ranking.size() > 5) {
            g_Ranking.resize(5);
        }
    }

    void Update(const Input& input) override {
        if (input.IsTrigger(KEY_INPUT_RETURN)) {
            m_pSceneManager->ChangeScene(SceneType::TITLE);
        }
    }

    void Draw() override {
        // 深緑ネオン背景
        DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(3, 20, 16), TRUE);

        DrawFormatStringToHandle(280, 80, GetColor(52, 211, 153), g_FontTitle, "MISSION CLEAR");
        DrawFormatStringToHandle(275, 130, GetColor(226, 232, 240), g_FontUI, "ENEMY CORE HAS BEEN DESTROYED.");

        // スコア＆アイテム
        DrawFormatStringToHandle(240, 190, GetColor(253, 224, 71), g_FontUI, "FINAL SCORE : %d PTS", m_pSceneManager->GetGameData().finalScore);
        DrawFormatStringToHandle(240, 220, GetColor(192, 132, 252), g_FontSmall, "RECOVERED NANOMACHINES : %d UNITS", m_pSceneManager->GetGameData().possessedItems);

        // スコアランキング表示板
        DrawBox(200, 270, 600, 480, GetColor(15, 23, 42), TRUE);
        DrawBox(200, 270, 600, 480, GetColor(52, 211, 153), FALSE);
        DrawFormatStringToHandle(300, 285, GetColor(253, 224, 71), g_FontUI, "-- ARCHIVE HIGHSCORES --");

        for (size_t i = 0; i < g_Ranking.size(); ++i) {
            unsigned int color = (g_Ranking[i].name == "PLAYER") ? GetColor(52, 211, 153) : GetColor(241, 245, 249);
            DrawFormatStringToHandle(250, 325 + (int)i * 26, color, g_FontSmall, "RANK %d   %-10s   %6d PTS", i + 1, g_Ranking[i].name.c_str(), g_Ranking[i].score);
        }

        DrawFormatStringToHandle(240, 520, GetColor(226, 232, 240), g_FontSmall, "[ PRESS ENTER TO RETURN TO MAIN ARCHIVE ]");
    }
};

// ============================================================================
// 【GameOverScene】ゲームオーバー画面
// ============================================================================
class GameOverScene : public Scene {
public:
    GameOverScene(SceneManager* manager) : Scene(manager) {}

    void Initialize() override {}

    void Update(const Input& input) override {
        if (input.IsTrigger(KEY_INPUT_RETURN)) {
            m_pSceneManager->ChangeScene(SceneType::TITLE);
        }
    }

    void Draw() override {
        // 深紅ダーク背景
        DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(24, 5, 5), TRUE);

        DrawFormatStringToHandle(290, 160, GetColor(239, 68, 68), g_FontTitle, "CONNECTION LOST");
        DrawFormatStringToHandle(230, 220, GetColor(226, 232, 240), g_FontUI, "PLAYER SHIELD CRITICAL: SYSTEM SHUTDOWN");

        DrawFormatStringToHandle(310, 300, GetColor(156, 163, 175), g_FontSmall, "RECORDED SCORE: %d PTS", m_pSceneManager->GetGameData().finalScore);

        DrawFormatStringToHandle(270, 420, GetColor(251, 146, 60), g_FontSmall, "[ PRESS ENTER TO REBOOT SYSTEM ]");
    }
};

// ============================================================================
// SceneManagerのメンバ関数の実体化（ChangeScene）
// ============================================================================
void SceneManager::ChangeScene(SceneType nextScene) {
    if (m_pCurrentScene != nullptr) {
        delete m_pCurrentScene;
        m_pCurrentScene = nullptr;
    }

    switch (nextScene) {
    case SceneType::TITLE:
        m_pCurrentScene = new TitleScene(this);
        break;
    case SceneType::GAME:
        m_pCurrentScene = new GameScene(this);
        break;
    case SceneType::RESULT:
        m_pCurrentScene = new ResultScene(this);
        break;
    case SceneType::GAMEOVER:
        m_pCurrentScene = new GameOverScene(this);
        break;
    }

    if (m_pCurrentScene != nullptr) {
        m_pCurrentScene->Initialize();
    }
}

// ============================================================================
// エントリーポイント（Windowsアプリケーションの開始点）
// ============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    ChangeWindowMode(TRUE);
    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);
    SetMainWindowText("Cyber Defender 3D - Robot Mech Template");

    if (DxLib_Init() == -1) {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    // --------------------------------------------------------------------
    // フォントアセットの生成 (アンチエイリアシングを有効化して美しく表示)
    // --------------------------------------------------------------------
    g_FontTitle = CreateFontToHandle("Consolas", 32, 3, DX_FONTTYPE_ANTIALIASING);
    g_FontUI = CreateFontToHandle("游ゴシック", 18, 4, DX_FONTTYPE_ANTIALIASING);
    g_FontSmall = CreateFontToHandle("游ゴシック", 15, 3, DX_FONTTYPE_ANTIALIASING);

    // 読み込みチェック
    if (g_FontTitle == -1 || g_FontUI == -1 || g_FontSmall == -1) {
        DxLib_End();
        return -1;
    }

    Input input;
    SceneManager sceneManager;
    sceneManager.ChangeScene(SceneType::TITLE);

    // メインループ
    while (ProcessMessage() == 0 && ClearDrawScreen() == 0) {
        input.Update();

        if (input.IsTrigger(KEY_INPUT_ESCAPE)) {
            break;
        }

        sceneManager.Update(input);
        sceneManager.Draw();

        ScreenFlip();
    }

    // 作成したフォントハンドルの解放
    DeleteFontToHandle(g_FontTitle);
    DeleteFontToHandle(g_FontUI);
    DeleteFontToHandle(g_FontSmall);

    DxLib_End();
    return 0;
}