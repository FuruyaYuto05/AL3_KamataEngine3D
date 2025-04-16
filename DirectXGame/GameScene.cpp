#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize(KamataEngine::Model* model_) {
	textureHandle_ = TextureManager::Load("AL3.jpg");
	model_ = Model::Create();
	worldTransform_, Initialize();
	camera_, Initialize();
}

void GameScene::Update() { 
	

}

void GameScene::Draw() {}

delete model_


