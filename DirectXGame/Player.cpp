#include "Player.h"
#include <KamataEngine.h>

void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) {
	model_ = model;
	textureHandle_ = textureHandle;
	camera_ = camera;

	assert(model_);

	 worldTransform_.scale_ = KamataEngine::Vector3(10.0f, 10.0f, 10.0f);
	worldTransform_.translation_ = KamataEngine::Vector3(0.0f, 0.0f, 0.0f); // カメラに対して適切な位置
	worldTransform_.Initialize();

	// 必ず初期化！
	worldTransform_.Initialize();
}

void Player::Update() { worldTransform_.TransferMatrix(); }

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