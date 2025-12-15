#include "Bullet.h"
#include <cassert>

// --- Bullet::Bullet ---
Bullet::Bullet() {}

// --- Bullet::~Bullet ---
Bullet::~Bullet() {}

// --- Bullet::Initialize ---
void Bullet::Initialize(Model* model, const Vector3& position, Player::LRDirection direction) {
	
	model_ = model;

	worldTransform_.Initialize();
	// プレイヤーの位置を初期位置に設定
	worldTransform_.translation_ = position;
	// Y軸回転を初期化 (今回は使わないかもしれないが、念のため)
	worldTransform_.rotation_.y = 0.0f;
	// スケールを小さめに設定
	worldTransform_.scale_ = {0.2f, 0.2f, 0.2f};

	// 弾の速度を設定
	if (direction == Player::LRDirection::kRight) {
		velocity_.x = kBulletSpeed;
	} else {
		velocity_.x = -kBulletSpeed;
	}
	velocity_.y = 0.0f;
	velocity_.z = 0.0f;

	isDead_ = false;
	lifeTime_ = 3.0f; // 3秒で消滅
}

// --- Bullet::Update ---
void Bullet::Update() {
	if (isDead_) {
		return;
	}

	const float deltaTime = 1.0f / 60.0f;

	// 1. 移動
	worldTransform_.translation_ += velocity_;

	// 2. ライフタイムを更新
	lifeTime_ -= deltaTime;

	// 3. ライフタイムが尽きたら消滅フラグを立てる
	if (lifeTime_ <= 0.0f) {
		isDead_ = true;
	}

	// 4. ワールド行列更新
	WorldTransformUpdate(worldTransform_);
}

// --- Bullet::Draw ---
void Bullet::Draw(const Camera& camera) { // <- Camera& camera を引数に追加
	if (!isDead_) {
		// 引数として受け取ったカメラを使用する
		model_->Draw(worldTransform_, camera);
	}
}

AABB Bullet::GetAABB() const {

	// 弾のワールド座標
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	// 弾サイズ（見た目と合わせる）
	const float halfSize = 0.2f;

	AABB aabb;
	aabb.min = {worldPos.x - halfSize, worldPos.y - halfSize, worldPos.z - halfSize};
	aabb.max = {worldPos.x + halfSize, worldPos.y + halfSize, worldPos.z + halfSize};

	return aabb;
}
