#include "Skydome.h"
#include "cassert"
void Skydome::Initialize(Model* model, Camera* camera) {
	// nullポインタチェック
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	camera_ = camera;
	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Skydome::Update() { worldTransform_.TransferMatrix(); }

void Skydome::Draw() { model_->Draw(worldTransform_, *camera_); }