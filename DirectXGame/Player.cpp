#include "Player.h"
#include <KamataEngine.h>
#include "MyMath.h"

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

void Player::Update() {
	// キャラの位置調整（ブロックと重ならない位置に変更）
	// ここでは例として位置を少し上にずらしていますが、ゲームロジックに応じて変更してください
	worldTransform_.translation_.x = 5.0f; // X座標を適切に変更
	worldTransform_.translation_.y = 1.0f; // Y座標を適切に変更
	worldTransform_.translation_.z = 5.0f; // Z座標を適切に変更

	// アフィン変換行列を再計算
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 定数バッファに行列を転送
	// TransferMatrix() メソッドで転送される行列は `matWorld_` を基に計算されるので、再計算した行列を転送する
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