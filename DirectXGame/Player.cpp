#include "Player.h"
#include <KamataEngine.h>
#include "MyMath.h"
#include <numbers>

void Player::Initialize(KamataEngine::Model* model,  KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	model_ = model;
	
	camera_ = camera;

	assert(model_);

	worldTransform_.scale_ = KamataEngine::Vector3(10.0f, 10.0f, 10.0f);
	worldTransform_.translation_ = position;
	worldTransform_.Initialize();

	
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;


	// 必ず初期化！
	worldTransform_.Initialize();
}

void Player::Update() {
	// キャラの位置調整（ブロックと重ならない位置に変更）
	
	worldTransform_.translation_.x = 5.0f; // X座標を適切に変更
	worldTransform_.translation_.y = 2.0f; // Y座標を適切に変更
	worldTransform_.translation_.z = 5.0f; // Z座標を適切に変更

	// アフィン変換行列を再計算
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 定数バッファに行列を転送
	
	worldTransform_.TransferMatrix();
}


void Player::Draw() {
	model_->Draw(worldTransform_, *camera_);
	
}

Player::~Player() {
	// リソースが不要ならばここで解放
	if (model_ != nullptr) {
		delete model_;
		model_ = nullptr;
	}
}