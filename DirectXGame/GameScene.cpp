#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("./Resources/AL3.jpg");

	// 3Dモデルの生成
	model_ = Model::Create();

	// カメラの初期化
	camera_.Initialize();

	// 自キャラの生成
	player_ = new Player();

	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_, &camera_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());

	// 自キャラの描画
	player_->Draw();

Model::PostDraw();
}

GameScene::~GameScene() {
	// モデルの解放
	delete model_;

	// player_ の解放
	if (player_ != nullptr) {
		delete player_;
		player_ = nullptr;
	}
}
