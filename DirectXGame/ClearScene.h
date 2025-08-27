#pragma once
#include "Fade.h"
#include "KamataEngine.h"

using namespace KamataEngine;

class ClearScene {
public:
	~ClearScene();

	void Initialize();
	void Update();
	void Draw();
	bool IsFinished() const { return finished_; }

private:
	Camera camera_;
	Model* modelClear_ = nullptr; // クリア表示用モデル
	WorldTransform worldTransformClear_;
	Fade* fade_ = nullptr;
	bool finished_ = false;
};
