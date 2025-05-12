#include "Skydome.h"
#include <KamataEngine.h>

void Skydome::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) { 
	model_ = model;
	textureHandle_ = textureHandle;
	camera_ = camera;

	assert(model_);

   worldTransform_.scale_ = KamataEngine::Vector3(500.0f, 500.0f, 500.0f);
	worldTransform_.translation_ = KamataEngine::Vector3(0.0f, 0.0f, 0.0f); // カメラに対して適切な位置
	worldTransform_.Initialize();
}

void Skydome::Update() {
	worldTransform_.TransferMatrix();

}

void Skydome::Draw() { 
	
		model_->Draw(worldTransform_, *camera_);
	

}

Skydome::~Skydome() { 
	if (model_ != nullptr) {
		delete model_;
		model_ = nullptr;
	}

}
