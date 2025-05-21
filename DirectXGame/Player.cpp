#include "Player.h"
#include "MyMath.h"
#include "cassert"
#include <numbers>
#include <KamataEngine.h>

using namespace MathUtility;

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	camera_ = camera;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {
	for (std::vector<KamataEngine::WorldTransform*>& worldTransformPlayerLine : worldTransformPlayer_) {
		for (KamataEngine::WorldTransform* worldTransformPlayer : worldTransformPlayerLine) {
			if (!worldTransformPlayer) {
				continue;
			}
			worldTransformPlayer->matWorld_ = MakeAffineMatrix(worldTransformPlayer->scale_, worldTransformPlayer->rotation_, worldTransformPlayer->translation_);
			worldTransformPlayer->TransferMatrix();
		}
	}

	// 移動入力
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		// 左右加速
		KamataEngine::Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			acceleration.x += kAcceleration;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			acceleration.x -= kAcceleration;
		}
		velocity_.x += acceleration.x;
	}
	
	// 移動
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

		// 行列の更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *camera_); }
