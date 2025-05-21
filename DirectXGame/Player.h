#pragma once
#include "KamataEngine.h"
#include <vector>

// 自キャラ
class Player {
private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	uint32_t textureHandle_ = 0u;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformPlayer_;

	KamataEngine::Vector3 velocity_ = {};

	static inline const float kAcceleration = 2;

public:
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera,const KamataEngine::Vector3& position);
	// 更新
	void Update();
	// 描画
	void Draw();
};
