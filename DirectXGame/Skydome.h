#pragma once
#include "KamataEngine.h"
#include <vector>

class Skydome {
public:
	void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera);

	void Update();

	void Draw();

	KamataEngine::Camera* camera_ = nullptr;

private:
	KamataEngine::WorldTransform worldTransform_;

	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	~Skydome();
};
