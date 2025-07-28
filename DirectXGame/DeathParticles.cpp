#include "DeathParticles.h"
#include <algorithm>

void DeathParticles::Initialize(Model* model, Camera* camera, const Vector3& position) {
	model_ = model;
	for (WorldTransform& worldTransform : worldTransform_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	camera_ = camera;

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void DeathParticles::Update() {
	// ワールド行列更新（アフィン変換～DirectXに転送）
	for (WorldTransform& worldTransform : worldTransform_) {
		WorldTransformUpdate(worldTransform);
	}

	for (uint32_t i = 0; i < 8; ++i) {
		Vector3 velocity = {0.1f, 0, 0};
		float angle = 20.0f * i;
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		velocity = Transform(velocity, matrixRotation);
		worldTransform_[i].translation_ += velocity;
	}
	counter_ += 1.0f / 60.0f;

	if (counter_ >= kDuration) {
		counter_ = kDuration;
		isFinished_ = true;
	}

	if (isFinished_) {
		return;
	}

	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);

	objectColor_.SetColor(color_);
}

void DeathParticles::Draw() {
	// モデル描画
	for (const WorldTransform& worldTransform : worldTransform_) {
		model_->Draw(worldTransform, *camera_,&objectColor_);
	}
	if (isFinished_) {
		return;
	}
}
