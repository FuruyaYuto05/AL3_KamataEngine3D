#include "RuleScene.h"
#include <cmath>
#include <numbers>
#include "MyMath.h"

RuleScene::~RuleScene() {
	delete sprite_;
	delete fade_;
	delete modelRule_;
	if (Audio::GetInstance()->IsPlaying(bgmHandle_)) {
		Audio::GetInstance()->StopWave(bgmHandle_);
	}

	
}

void RuleScene::Initialize() {
	//textureHandle_ = TextureManager::Load("ruleScene.png");
	textureHandleRuleGuide_ = TextureManager::Load("playing.png");
	//textureHandleArrowRight_ = TextureManager::Load("GuideArrowRight.png");
	//textureHandleArrowLeft_ = TextureManager::Load("GuideArrowLeft.png");

	modelRule_ = Model::CreateFromOBJ("ruleFont", true);

	// カメラ初期化
	camera_.Initialize();

	const float kPlayerRule = 2.0f;

	worldTransformRule_.Initialize();

	worldTransformRule_.scale_ = {kPlayerRule, kPlayerRule, kPlayerRule};

	sprite_ = Sprite::Create(textureHandle_, {0, 0});
	spriteRuleGuide_ = Sprite::Create(textureHandleRuleGuide_, {0, 0});
	spriteArrowRight_ = Sprite::Create(textureHandleArrowRight_, {1100, 300});
	spriteArrowLeft_ = Sprite::Create(textureHandleArrowLeft_, {100, 300});

	fade_ = new Fade();
	fade_->Initialize();

	fade_->Start(Fade::Status::FadeIn, 1.0f, Fade::Type::kShutter);

	bgmDataHandle_ = Audio::GetInstance()->LoadWave("rule.wav");


}

void RuleScene::Update() {
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
			fade_->Start(Fade::Status::FadeOut, 1.0f, Fade::Type::kShutter);
			phase_ = Phase::kFadeOut;
			isPlayBgm_ = false;
			if (!isPlaySe_) {
				seHandle_ = Audio::GetInstance()->PlayWave(enterSeDataHandle_, false);
			}
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

	camera_.TransferMatrix();
	WorldTransformUpdate(worldTransformRule_);
}

void RuleScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);

	
	spriteRuleGuide_->Draw();
	

	Sprite::PostDraw();

    Model::PreDraw(dxCommon_->GetCommandList());

	modelRule_->Draw(worldTransformRule_, camera_);

	fade_->Draw();

	Model::PostDraw();
}

void RuleScene::PlayBgm() {
	if (!isPlayBgm_) {
		bgmHandle_ = Audio::GetInstance()->PlayWave(bgmDataHandle_, true, 0.3f);
		isPlayBgm_ = true;
	}
}

void RuleScene::PlaySe() {}