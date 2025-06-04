#pragma once
#include "KamataEngine.h"
#include <vector>

using namespace KamataEngine;

// 自キャラ
class Player {
private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	uint32_t textureHandle_ = 0u;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformPlayer_;

	KamataEngine::Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.5f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed = 1.0f;

	enum class LRDirection 
	{ 
		kRight,
		kLeft,
	};

	LRDirection lrDirection_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;

	static inline const float kTimeTurn = 0.3f;

	bool onGround_ = true;
	static inline const float kGravityAcceleration = 0.1f;
	static inline const float kLimitFallSpeed = 0.7f;
	static inline const float kJumpAcceleration = 1.0f;

	

public:
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera,const KamataEngine::Vector3& position);
	// 更新
	void Update();
	// 描画
	void Draw();

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
};
