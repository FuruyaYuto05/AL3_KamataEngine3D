#include "ClearScene.h"
#include "MyMath.h"

ClearScene::~ClearScene() {
	delete modelClear_;
	delete fade_;
}

void ClearScene::Initialize() {
	modelClear_ = Model::CreateFromOBJ("clearFont", true); // clear.obj を用意
	camera_.Initialize();

	worldTransformClear_.Initialize();
	worldTransformClear_.translation_ = {0, 0, 0};
	worldTransformClear_.scale_ = {10.0f, 10.0f, 10.0f}; // 大きく表示

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void ClearScene::Update() {
	fade_->Update();

	// エンターキーでタイトルに戻す
	if (fade_->IsFinished() && Input::GetInstance()->PushKey(DIK_RETURN)) {
		finished_ = true;
	}

	// カメラ更新
	camera_.TransferMatrix();

	// ワールド行列更新
	WorldTransformUpdate(worldTransformClear_);
}

void ClearScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	Model::PreDraw(cmdList);
	modelClear_->Draw(worldTransformClear_, camera_);
	Model::PostDraw();

	fade_->Draw();
}
