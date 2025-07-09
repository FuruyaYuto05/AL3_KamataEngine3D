#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include "CameraController.h"
#include "Enemy.h"
#include <vector>

class GameScene {

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera camera_;
	Player* player_ = nullptr;

	Enemy* enemy_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	KamataEngine::Model* modelSkydome_ = nullptr;

	KamataEngine::Model* modelPlayer_ = nullptr;

	KamataEngine::Model* modelEnemy_ = nullptr;

	Skydome* skydome_ = nullptr;

	KamataEngine::Model* modelCameracontoroller_ = nullptr;

	CameraController* cameraController_;


	MapChipField* mapChipField_;
	void GenerateBlocks();
	
private:
	uint32_t textureHandle_ = 0;
	KamataEngine::Model* modelBlock_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

public:
	// 初期化
	void Initialize();
	~GameScene();
	// 更新
	void Update();
	// 描画
	void Draw();
};
