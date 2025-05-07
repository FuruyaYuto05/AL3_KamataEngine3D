#pragma once
#include "KamataEngine.h"
#include <vector>


//ゲームシーン
class GameScene {
public:
	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	// 3Dモデル
	KamataEngine::Model* modelBlock_ = nullptr;

	// カメラ
	KamataEngine::Camera camera_;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	bool isDebugCameraActive_ = false;

	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	

	~GameScene();

};

