#include "Goal.h"
#include "MyMath.h"

void Goal::Initialize(const Vector3& position, Model* model, Camera* camera) {
	position_ = position;
	model_ = model;
	camera_ = camera;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position_;
}

void Goal::Update() { WorldTransformUpdate(worldTransform_); }

void Goal::Draw() { model_->Draw(worldTransform_, *camera_); }

bool Goal::IsPlayerReached(const Vector3& playerPos) const {
	float width = 1.0f;
	float height = 1.0f;

	return (playerPos.x > position_.x - width / 2.0f && playerPos.x < position_.x + width / 2.0f && playerPos.y > position_.y - height / 2.0f && playerPos.y < position_.y + height / 2.0f);
}
