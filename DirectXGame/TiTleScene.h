#pragma once
#include "KamataEngine.h"

#include "Fade.h"

using namespace KamataEngine;



// 02_12 19枚目 タイトルシーン
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

	// 02_12 26枚目
	bool IsFinished() const { return finished_; }

	



private:
	static inline const float kTimeTitleMove = 2.0f;

	// ビュープロジェクション
	Camera camera_;
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformPlayer_;

	Model* modelPlayer_ = nullptr;
	Model* modelTitle_ = nullptr;

	float counter_ = 0.0f;
	// 02_12 26枚目
	bool finished_ = false;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;

};