#pragma once
#include "KamataEngine.h"
#include "MyMath.h"
#include "Player.h" // プレイヤーの方向を取得するために含める

using namespace KamataEngine;

class Bullet {
public:
	// コンストラクタ
	Bullet();
	// デストラクタ
	~Bullet();

	/// 初期化
	void Initialize(Model* model, const Vector3& position, Player::LRDirection direction);

	/// 更新
	void Update();

	/// 描画
	void Draw(const Camera& camera);

	// 弾が消滅しているかを取得
	bool IsDead() const { return isDead_; }

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	// サイズ調整
	void SetScale(const Vector3& scale) { worldTransform_.scale_ = scale; }

	// スピード調整
    void SetSpeed(float speed) {
		if (velocity_.x > 0)
			velocity_.x = speed;
		else
			velocity_.x = -speed;
	}

	// モデル設定
	void SetModel(Model* model) { model_ = model; }

	// 弾の当たり判定
	AABB GetAABB() const;

	// 弾を消す
	void SetDead(bool dead) { isDead_ = dead; }


private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// 弾の速度
	Vector3 velocity_ = {};

	// 弾が消滅しているか
	bool isDead_ = false;

	// 弾の生存時間 (秒)
	float lifeTime_ = 3.0f;
	// 弾の移動速度
	static inline const float kBulletSpeed = 0.3f;
};