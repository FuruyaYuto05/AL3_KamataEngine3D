#include "GameOver.h"
#include <numbers>
#include "MyMath.h"

GameOver::~GameOver() {
	delete modelGameOver_;
	delete fade_;
	if (Audio::GetInstance()->IsPlaying(bgmHandle_)) {
		Audio::GetInstance()->StopWave(bgmHandle_);
	}
}

void GameOver::Initialize() {

	modelGameOver_ = Model::CreateFromOBJ("gameoverFont", true);

	camera_.Initialize();

	worldTransformClear_.Initialize();
	worldTransformClear_.scale_ = {2.0f, 2.0f, 2.0f};

	fade_ = new Fade();
	fade_->Initialize();

	fade_->Start(Fade::Status::FadeIn, 1.0f);

	// 天球
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	camera_.TransferMatrix();
	WorldTransformUpdate(worldTransformClear_);

	bgmDataHandle_ = Audio::GetInstance()->LoadWave("gameover.wav");
	seDataHandle_ = Audio::GetInstance()->LoadWave("enter.wav");
}

void GameOver::Update() {
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
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
			PlaySe();
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

	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeClearMove);

	float angle = counter_ / kTimeClearMove * 2.0f * std::numbers::pi_v<float>;

	worldTransformClear_.translation_.y = std::sin(angle) + 10.0f;

	skydome_->Update();
}

void GameOver::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon_->GetCommandList());


	modelGameOver_->Draw(worldTransformClear_, camera_);
	skydome_->Draw();

	fade_->Draw();

	Model::PostDraw();
}

void GameOver::PlayBgm() {
	if (!isPlayBgm_) {
		bgmHandle_ = Audio::GetInstance()->PlayWave(bgmDataHandle_, true, 0.3f);
		isPlayBgm_ = true;
	}
}

void GameOver::PlaySe() {
	if (!isPlaySe_) {
		seHandle_ = Audio::GetInstance()->PlayWave(seDataHandle_, false);
		isPlaySe_ = true;
	}
}
