#pragma once
#include "KamataEngine.h"
class Enemy {

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	// マップチップによるフィールド
	//MapChipField* mapChipField_ = nullptr;

	float walkTimer_ = 0.0f;

public:
	
	static inline const float kWalSpeed = 0.05f;

	KamataEngine::Vector3 velocity_ = {};

	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	// 更新
	void Update();
	//void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	// 描画
	void Draw();
};
