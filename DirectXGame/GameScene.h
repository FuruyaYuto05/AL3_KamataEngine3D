#pragma once
#include "KamataEngine.h"
#include "Skydome.h"
#include "Player.h"
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
	KamataEngine::Model* modelSkydome_ = nullptr;
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera camera_;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	bool isDebugCameraActive_ = false;

	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	Skydome* skydome_ = nullptr;

	Player* player_ = nullptr;

	~GameScene();

	private:
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;


};

