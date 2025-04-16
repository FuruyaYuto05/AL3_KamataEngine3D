#pragma once
#include "KamataEngine.h"

//ゲームシーン
class GameScene {
private:
	unit32_t textureHandle_ = 0;
	

public:
	//初期化
	void Initialize(
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_; 
	KamataEngine::Camera camera_;
	);

	//更新
	void Update();

	//描画
	void Draw();

	


};

