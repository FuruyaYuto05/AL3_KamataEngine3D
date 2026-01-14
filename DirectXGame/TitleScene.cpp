#include "TitleScene.h"
#include <cmath>
#include <numbers>
#include "MyMath.h"

using namespace KamataEngine;
TitleScene::~TitleScene() {
	delete modelTitle_;
	delete modelPlayerRight_;
	delete modelPlayerLeft_;
	delete fade_;
	if (Audio::GetInstance()->IsPlaying(bgmHandle_)) {
		Audio::GetInstance()->StopWave(bgmHandle_);
	}
}

void TitleScene::Initialize() {
	modelTitle_ = Model::CreateFromOBJ("titleFont", true);
	// modelPlayerRight_ = Model::CreateFromOBJ("playerRight", true);
	// modelPlayerLeft_ = Model::CreateFromOBJ("playerLeft", true);
	//modelGuid_ = Model::CreateFromOBJ("guid", true);

	// カメラ初期化
	camera_.Initialize();

	const float kPlayerTitle = 2.0f;

	worldTransformTitle_.Initialize();

	worldTransformTitle_.scale_ = {kPlayerTitle, kPlayerTitle, kPlayerTitle};

	const float kPlayerScale = 5.0f;

	worldTransformPlayerRight_.Initialize();
	worldTransformPlayerRight_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};
	worldTransformPlayerRight_.rotation_.y = 1.50f * std::numbers::pi_v<float>;
	worldTransformPlayerRight_.translation_ = {-50.0f, -10.0f};

	worldTransformPlayerLeft_.Initialize();
	worldTransformPlayerLeft_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};
	worldTransformPlayerLeft_.rotation_.y = 1.50f * std::numbers::pi_v<float>;
	worldTransformPlayerLeft_.translation_ = {-50.0f, -10.0f};

	// タイトルのY座標(10.0f)から下にオフセット (-5.0f は調整可能)
	worldTransformGuid_.translation_ = {
	    worldTransformTitle_.translation_.x + 20.0f,
	    worldTransformTitle_.translation_.y - 15.0f, // ★ Y座標を下げて配置
	    worldTransformTitle_.translation_.z};

	// ★ 操作ガイド用のワールド変換を初期化し、位置を下げる
	worldTransformGuid_.Initialize();
	worldTransformGuid_.scale_ = {1.5f, 1.5f, 1.5f}; // スケールも調整可能

	fade_ = new Fade();
	fade_->Initialize();

	fade_->Start(Fade::Status::FadeIn, 1.0f);

	// 天球
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	bgmDataHandle_ = Audio::GetInstance()->LoadWave("title.wav");
	seDataHandle_ = Audio::GetInstance()->LoadWave("enter.wav");
}

void TitleScene::Update() {

	skydome_->Update();

#pragma region フェード
	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
			PlayBgm();
		}
		break;

	case Phase::kMain:

		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
			isPlayBgm_ = false;

			// PlaySe();
		}
		break;

	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
#pragma endregion

	// counter_ += 1.0f / 60.0f;
	// counter_ = std::fmod(counter_, kTimeTitleMove);

	// float angle = counter_ / kTimeTitleMove * 2.0f * std::numbers::pi_v<float>;

	// worldTransformTitle_.translation_.y = std::sin(angle) + 10.0f;

	// #pragma region タイトルシーンの動き
	//
	//	// 右向きのやつ
	//	worldTransformPlayerRight_.translation_.x += 0.2f;
	//
	//	if (worldTransformPlayerRight_.translation_.x >= 0.0f) {
	//
	//		if (worldTransformPlayerRight_.translation_.y >= 15.0f) {
	//
	//		} else {
	//			worldTransformPlayerRight_.translation_.y += 0.9f;
	//		}
	//	}
	//
	//	if (worldTransformPlayerRight_.translation_.x >= 50.0f) {
	//		worldTransformPlayerRight_.translation_ = {-50.0f, -10.0f};
	//	}
	//
	//	// 左向きのやつ
	//	worldTransformPlayerLeft_.translation_.x -= 0.15f;
	//
	//	if (worldTransformPlayerLeft_.translation_.x <= 0.0f) {
	//		if (worldTransformPlayerLeft_.translation_.y >= 15.0f) {
	//		} else {
	//			worldTransformPlayerLeft_.translation_.y += 0.9f;
	//		}
	//	}
	//
	//	if (worldTransformPlayerLeft_.translation_.x <= -50.0f) {
	//		worldTransformPlayerLeft_.translation_ = {50.0f, -10.0f};
	//	}
	//
	// #pragma endregion

	camera_.TransferMatrix();
	WorldTransformUpdate(worldTransformTitle_);
	WorldTransformUpdate(worldTransformPlayerRight_);
	WorldTransformUpdate(worldTransformPlayerLeft_);
	WorldTransformUpdate(worldTransformGuid_);
}

void TitleScene::Draw() {

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon_->GetCommandList());


	skydome_->Draw();

	// modelPlayerRight_->Draw(worldTransformPlayerRight_, camera_);
	// modelPlayerLeft_->Draw(worldTransformPlayerLeft_, camera_);
	modelTitle_->Draw(worldTransformTitle_, camera_);

	//modelGuid_->Draw(worldTransformGuid_, camera_);

	fade_->Draw();

	Model::PostDraw();
}

void TitleScene::PlayBgm() {
	if (!isPlayBgm_) {
		bgmHandle_ = Audio::GetInstance()->PlayWave(bgmDataHandle_, true, 0.3f);
		isPlayBgm_ = true;
	}
}

void TitleScene::PlaySe() {
	if (!isPlaySe_) {
		seHandle_ = Audio::GetInstance()->PlayWave(seDataHandle_, false);
		isPlaySe_ = true;
	}
}
