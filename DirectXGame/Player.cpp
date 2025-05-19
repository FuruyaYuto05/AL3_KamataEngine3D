#include "Player.h"
#include <cassert>
using namespace KamataEngine;

void Player::Initialize(Model* model, uint32_t textureHandle, Camera* camera) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	camera_ = camera;
}
void Player::Update() { worldTransform_.TransferMatrix(); }
void Player::Draw() { model_->Draw(worldTransform_, *camera_); }

Player::~Player() {
	// リソースが不要ならばここで解放
	if (model_ != nullptr) {
		delete model_;
		model_ = nullptr;
	}
}