#pragma once
#include "Fade.h"
#include "KamataEngine.h"
#include "Math.h"
#include "Skydome.h"

using namespace KamataEngine;
class TitleScene {
public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	~TitleScene();
	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return finished_; }

	// bgm
	void PlayBgm();
	void PlaySe();

private:
	static inline const float kTimeTitleMove = 2.0f;

	Camera camera_;
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformPlayerLeft_;
	WorldTransform worldTransformPlayerRight_;

	Model* modelTitle_;
	Model* modelPlayerRight_;
	Model* modelPlayerLeft_;

	Model* modelGuid_;

	// ★ 操作ガイド用のワールド変換を追加
	WorldTransform worldTransformGuid_;

	float counter_ = 0.0f;

	bool finished_ = false;

	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;

	Model* modelSkydome_ = nullptr;
	Skydome* skydome_ = nullptr;
	// bgm
	//uint32_t bgmDataHandle_ = 0;
	//uint32_t bgmHandle_ = 0;
	//bool isPlayBgm_ = false;

	uint32_t seDataHandle_;
	uint32_t seHandle_;
	bool isPlaySe_ = false;
};
