#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

using namespace KamataEngine;

class EnemyBullet {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度ベクトル</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(const Camera& camera);

	// 死亡フラグ（寿命や衝突でtrueにする）
	bool IsDead() const { return isDead_; }

	// 衝突判定用：ワールド座標取得
	Vector3 GetWorldPosition() const;

	// 衝突時のコールバック
	void OnCollision();

	AABB GetAABB();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Vector3 velocity_ = {};

	// 寿命タイマー
	static const int32_t kLifeTime = 60 * 5; // 5秒
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

	static inline const float kWidth = 0.2f; // 弾のサイズ
	static inline const float kHeight = 0.2f;
};