#pragma once
#include "Fade.h"
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;
class RuleScene {
public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	~RuleScene();
	void Initialize();
	void Update();
	void Draw();
	bool IsFinished() const { return finished_; }

	void PlayBgm();
	void PlaySe();

private:
	bool finished_ = false;

	Camera camera_;
	WorldTransform worldTransformRule_;

	Model* modelRule_;

	Sprite* sprite_ = nullptr;
	uint32_t textureHandle_ = 0;

	Sprite* spriteRuleGuide_ = nullptr;
	uint32_t textureHandleRuleGuide_ = 0;

	Sprite* spriteArrowRight_ = nullptr;
	uint32_t textureHandleArrowRight_ = 0;

	Sprite* spriteArrowLeft_ = nullptr;
	uint32_t textureHandleArrowLeft_ = 0;

	bool drawingPicture_ = false;

	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;

	uint32_t bgmDataHandle_;
	uint32_t bgmHandle_;
	bool isPlayBgm_ = false;

	uint32_t enterSeDataHandle_;
	uint32_t moveSeDataHandle_;
	uint32_t seHandle_;
	bool isPlaySe_ = false;
};