#include "Player.h"
#include <KamataEngine.h>

void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) {
	model_ = model;
	textureHandle_ = textureHandle;
	camera_ = camera;

	assert(model_);

// 必ず初期化！
	worldTransform_.Initialize();
}



void Player::Update() {
    worldTransform_.TransferMatrix();
}

void Player::Draw() {
    if (model_ != nullptr) {
        model_->Draw(worldTransform_,*camera_,textureHandle_);
    }
}

Player::~Player() {
    // リソースが不要ならばここで解放
    if (model_ != nullptr) {
        delete model_;
        model_ = nullptr;
    }
}
