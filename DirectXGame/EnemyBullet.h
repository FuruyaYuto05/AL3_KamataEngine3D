#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

using namespace KamataEngine;

class EnemyBullet {
public:
	
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const Camera& camera);

	// 死亡フラグ（寿命や衝突でtrueにする）
	bool IsDead() const { return isDead_; }

	// ワールド座標取得
	Vector3 GetWorldPosition() const;

	// 衝突時のコールバック
	void OnCollision();

	AABB GetAABB();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Vector3 velocity_ = {};

	// 寿命タイマー
	static const int32_t kLifeTime = 60 * 3; // 3秒
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

	static inline const float kWidth = 0.2f; // 弾のサイズ:横
	static inline const float kHeight = 0.2f;// 弾のサイズ:縦
};