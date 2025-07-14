#include "Enemy.h"
#include "MyMath.h"
#include <numbers>

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {

assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	camera_ = camera;
	velocity_ = {-kWalSpeed, 0, 0};
	walkTimer_ = 0.0f;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;
}

void Enemy::Update() {

	worldTransform_.translation_.x += velocity_.x;

	//タイマーを加算
	walkTimer_ += 1.0f;

// 回転アニメーション：サイン波でx軸回転（±45度）
	worldTransform_.rotation_.x = std::sin(walkTimer_ * 0.1f) * (std::numbers::pi_v<float> / 4.0f);

	// 行列の更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Enemy::Draw() {

	model_->Draw(worldTransform_, *camera_);
}
