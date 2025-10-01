#pragma once
#include "KamataEngine.h"
using namespace KamataEngine;

class Goal {
public:
	void Initialize(const Vector3& position, Model* model, Camera* camera);
	void Update();
	void Draw();

	Vector3 GetPosition() const { return position_; }
	bool IsPlayerReached(const Vector3& playerPos) const;

	// 追加：ワールド変換を取得する
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

private:
	Vector3 position_ = {};
	Model* model_ = nullptr;
	Camera* camera_ = nullptr;
	WorldTransform worldTransform_;
};
