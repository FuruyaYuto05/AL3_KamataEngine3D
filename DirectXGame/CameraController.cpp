#include "CameraController.h"
#include "Player.h"

using namespace MathUtility;

void CameraController::Initialize() {
	camera_.Initialize(); 
}

void CameraController::Update() {

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	const Vector3& targetVelocity = target_->GetVelocity();

	targetPosition_ = targetWorldTransform.translation_ + targetOffset_ + targetVelocity * kVelocityBias;

	//座標補間によりゆったり追従
	camera_.translation_.x = Lerp(camera_.translation_.x, targetPosition_.x, kInterpolationRate);

	//追従対象が画面外に出ないように補正
	camera_.translation_.x = max(camera_.translation_.x, targetPosition_.x + targetMargin.left);
	camera_.translation_.x = min(camera_.translation_.x, targetPosition_.x + targetMargin.right);
	camera_.translation_.y = max(camera_.translation_.y, targetPosition_.y + targetMargin.bottom);
	camera_.translation_.y = min(camera_.translation_.y, targetPosition_.y + targetMargin.top);

	//移動範囲制限
	camera_.translation_.x = max(camera_.translation_.x,movableArea_.left);
	camera_.translation_.x = min(camera_.translation_.x, movableArea_.right);
	camera_.translation_.y = max(camera_.translation_.y, movableArea_.bottom);
	camera_.translation_.y = min(camera_.translation_.y, movableArea_.top);


	camera_.UpdateMatrix();
}

void CameraController::Reset() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;
}
