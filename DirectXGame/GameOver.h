#pragma once
#include "Fade.h"
#include "KamataEngine.h"
#include "Math.h"
#include "Skydome.h"

using namespace KamataEngine;
class GameOver {

public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	~GameOver();
	void Initialize();
	void Update();
	void Draw();
	bool IsFinished() const { return finished_; }

	void PlayBgm();
	void PlaySe();

private:
	static inline const float kTimeClearMove = 2.0f;

	Camera camera_;
	WorldTransform worldTransformClear_;

	Model* modelGameOver_;
	float counter_ = 0.0f;

	bool finished_ = false;

	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;

	uint32_t bgmDataHandle_ = 0;
	uint32_t bgmHandle_ = 0;
	bool isPlayBgm_ = false;

	uint32_t seDataHandle_;
	uint32_t seHandle_;
	bool isPlaySe_ = false;

	Model* modelSkydome_ = nullptr;
	Skydome* skydome_ = nullptr;
};
