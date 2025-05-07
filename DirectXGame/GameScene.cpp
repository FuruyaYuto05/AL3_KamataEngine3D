#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	// 3Dモデルの生成
	modelBlock_ = Model::Create();

	const uint32_t kNumBlockHorizontal = 20;

	const float kBlockWidth = 2.0f;
	worldTransformBlocks_.resize(kNumBlockHorizontal);

	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i) {

		worldTransformBlocks_[i] = new WorldTransform();
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.0f;
	}
}

void GameScene::Update() {

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
	

		worldTransformBlock->TransferMatrix();
	}

}

void GameScene::Draw() {}
GameScene::~GameScene() {

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
	}
	worldTransformBlocks_.clear();
}