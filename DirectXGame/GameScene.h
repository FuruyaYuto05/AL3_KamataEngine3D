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

	std::vector<KamataEngine::WorldTransform*> worldTransformBlocks_;

	~GameScene();

};

