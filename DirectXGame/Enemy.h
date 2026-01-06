#pragma once

#include "KamataEngine.h"
#include "MyMath.h"
#include "EnemyBullet.h"
#include <list>
#include "MapChipField.h"
#include "DeathParticles.h"


using namespace KamataEngine;

// 02_10 20枚目
class Player;

// 02_09 スライド4枚目
class Enemy {

public:
	// 02_09 スライド5枚目
	void Initialize(Model* model, Camera* camera, const Vector3& position);
	// 02_09 スライド5枚目
	void Update();
	// 02_09 スライド5枚目
	void Draw();
	// 02_10 スライド14枚目
	AABB GetAABB();
	// 02_10 スライド14枚目 ワールド座標を取得
	Vector3 GetWorldPosition();
	// 02_10 スライド20枚目 衝突応答
	void OnCollision(const Player* player);
	// --- プレイヤーの攻撃を受けた際の応答 ---
	void OnHitByPlayerAttack(const Player* player);


	// プレイヤーをセットする関数を追加
	void SetPlayer(Player* player) { player_ = player; }

	// モデルをセットする関数を追加
	void SetBulletModel(Model* model) { bulletModel_ = model; }

	// 発射した弾のリストを取得（当たり判定などで使う）
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	~Enemy();

	void SetMapChipField(MapChipField* map) { mapChipField_ = map; }

	MapChipField* mapChipField_ = nullptr;

	enum Corner { kRightBottom, kLeftBottom, kRightTop, kLeftTop, kNumCorner };

	bool IsDead() const { return isDead_; }

	void SetDeathParticleModel(Model* model) { deathParticleModel_ = model; }

	bool IsDying() const { return isDying_; }
	bool IsReadyToRemove() const { return isDead_; }

	//int hp_ = 3;
	bool isDying_ = false; // 演出中
	//bool isDead_ = false;  // 演出終了後に削除OK

	Model* deathParticleModel_ = nullptr;
	DeathParticles* deathParticles_ = nullptr;

	void StartDeath();

	int deathTimer = 20;

	int32_t GetHP() const { return hp_; }


private:
	// 02_09 6枚目 ザ・ワールド
	WorldTransform worldTransform_;
	// 02_09 6枚目 モデル
	Model* model_ = nullptr;
	// 02_09 6枚目 カメラ
	Camera* camera_ = nullptr;

	// 02_09 15枚目
	static inline const float kWalkSpeed = 0.02f;
	// 02_09 15枚目
	Vector3 velocity_ = {};

	// 02_09 19枚目
	static inline const float kWalkMotionAngleStart = 0.0f;
	// 02_09 19枚目
	static inline const float kWalkMotionAngleEnd = 30.0f;
	// 02_09 19枚目
	static inline const float kWalkMotionTime = 1.0f;
	// 02_09 20枚目
	float walkTimer = 0.0f;

	// 02_10 14枚目 当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.04f;

	// 発射機能用
	Player* player_ = nullptr;        // プレイヤーへのポインタ
	std::list<EnemyBullet*> bullets_; // 発射した弾のリスト
	Model* bulletModel_ = nullptr;

	static inline const int kFireInterval = 120; // 発射間隔
	int32_t fireTimer_ = 0;
	static inline const float kBulletSpeed = 0.1f; // 弾の速度

	// 弾を発射する関数
	void Fire();

	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move;
	};

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	void CheckMapCollision(CollisionMapInfo& info);

	void CheckMapCollisionDown(CollisionMapInfo& info);
	//void CheckMapCollisionUP(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	// 設置状態の切り替え処理
	void UpdateOnGround(const CollisionMapInfo& info);

	// 壁接触している場合の処理
	void UpdateOnWall(const CollisionMapInfo& info);

	//static inline const float kAttenuationWall = 0.2f;
	static inline const float kAttenuationLanding = 0.0f;
	static inline const float kGroundSearchHeight = 0.06f;
	//bool can2Jump_ = false;
	bool onGround_ = true;

	//playerの追尾(まだ)
	bool tracked_ = true;
	static inline const float kJumpAcceleration = 20.0f;
	static inline const float kGravityAcceleration = 0.98f;
	static inline const float kLimitFallSpeed = 0.5f;
	// 最高速度
	static inline const float kLimitRunSpeed = 0.3f;

	// HP関連
	int hp_ = 3;          // 敵のHP
	bool isDead_ = false; // 死亡フラグ

	// ノックバック関連
	bool isKnockback_ = false;
	float knockbackTimer_ = 0.0f;
	static inline const float kKnockbackDuration = 0.2f; 
	Vector3 knockbackVelocity_ = {};

	
};
