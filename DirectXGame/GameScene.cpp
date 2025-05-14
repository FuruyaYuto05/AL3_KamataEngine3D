#include "GameScene.h"
#include "MyMath.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	// 3Dモデルの生成
	modelBlock_ = Model::CreateFromOBJ("block");
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	model_ = Model::CreateFromOBJ("player");

	// カメラの初期化
	camera_.Initialize();

	debugCamera_ = new DebugCamera(1280, 720);

	const uint32_t kNumBlockVirtical = 20;
	const uint32_t kNumBlockHorizontal = 100;

	

	skydome_ = new Skydome();
	player_ = new Player();

	textureHandle_ = TextureManager::Load("./Resources/SkyDome/sky_sphere.png");


	skydome_->Initialize(modelSkydome_, textureHandle_, &camera_);

	player_->Initialize(model_, textureHandle_, &camera_);

	worldTransformBlocks_.resize(kNumBlockVirtical);


	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);


			}
			
		}
	}
}

void GameScene::Update() {

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) 
				continue;

				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				worldTransformBlock->TransferMatrix();
		}
	}

	debugCamera_->Update();

	#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

    #endif // _DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.UpdateMatrix();
	}

}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());
	skydome_->Draw();  
	player_->Draw();
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			
			modelBlock_->Draw(*worldTransformBlock, camera_);
			model_->Draw(*worldTransformBlock, camera_);
		}
	}
}

void GameScene::GenerateBlocks() {
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
}



GameScene::~GameScene() {

	delete modelBlock_;
	delete debugCamera_;
	delete modelSkydome_;
	delete model_;
	delete mapChipField_;


	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}