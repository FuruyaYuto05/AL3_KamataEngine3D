#pragma once

#include "KamataEngine.h"

using namespace KamataEngine;
using namespace MathUtility;

class Player;

class CameraController {
private:
	// カメラ
	KamataEngine::Camera camera_;

	Player* target_ = nullptr;

	Vector3 targetOffset_ = {0, 0, -15.0f};

	Player* player_;


	// 座標補間割合
	static inline const float kInterpolationRate = 1.0f;

	//速度掛け率
	static inline const float kVelocityBias = 1;

	

public:
	void Initialize();

	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	Rect movableArea_ = {0, 100, 0, 100};

	void SetMovableArea(Rect area) { movableArea_ = area; }

	//カメラの目標座標
	KamataEngine::Vector3 targetPosition_;

	// 追従対象の各方向へのカメラ移動範囲
	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};

	const KamataEngine::Camera& GetViewProjection() const { return camera_; }
};
