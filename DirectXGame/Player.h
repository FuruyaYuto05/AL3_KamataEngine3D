#pragma once

#include <KamataEngine.h>


// ゲームシーン
class Player {
private:
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	

public:
	// 初期化
	void Initialize(KamataEngine::Model* model,  KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	KamataEngine::Camera* camera_ = nullptr;

	~Player();
};